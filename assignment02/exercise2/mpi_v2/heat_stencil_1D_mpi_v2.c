#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define RESOLUTION 120

// -- vector utilities --

typedef double *Vector;

Vector createVector(int N);

void releaseVector(Vector m);

void printTemperature(Vector m, int N);

// -- simulation code ---

int main(int argc, char **argv) {
  // 'parsing' optional input parameter = problem size
  int N = 2000;
  if (argc > 1) {
    N = atoi(argv[1]);
  }
  int T = N * 500;
  printf("Computing heat-distribution for room size N=%d for T=%d timesteps\n", N, T);

  // ---------- setup ----------

  // create a buffer for storing temperature fields
  Vector A = createVector(N);

  // MPI init
  int rank_id, number_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_ranks);

  int from = rank_id * N / number_of_ranks;
  int to = (rank_id + 1) * N / number_of_ranks;
  struct timespec start, stop;

  if (rank_id == 0){ // do setup only on rank 0
    // set up initial conditions in A
    for (int i = 0; i < N; i++) {
      A[i] = 273; // temperature is 0° C everywhere (273 K)
    }

    // and there is a heat source in one corner
    A[N / 4] = 273 + 60;

    printf("Initial:\t");
    printTemperature(A, N);
    printf("\n");

    clock_gettime(CLOCK_REALTIME, &start);
  }

  // ---------- compute ----------

  Vector B = createVector(N);
  // for each time step ..
  for (int t = 0; t < T; t++) {
    MPI_Bcast(A, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // .. we propagate the temperature
    for (long long i = from; i < to; i++) {
      // center stays constant (the heat is still on)
      if (i == N/4) {
        B[i] = A[i];
        continue;
      }

      // get temperature at current position
      double tc = A[i];

      // get temperatures of adjacent cells
      double tl = (i != 0) ? A[i - 1] : tc;
      double tr = (i != N - 1) ? A[i + 1] : tc;

      // compute new temperature at current position
      B[i] = tc + 0.2 * (tl + tr + (-2 * tc));
    }
    
    MPI_Reduce(B, A, N, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // show intermediate step
    if ( rank_id == 0 && !(t % 1000)) {
      printf("Step t=%d:\t", t);
      printTemperature(A, N);
      printf("\n");
    }
    
  }

  releaseVector(B);

  if(rank_id == 0){

  }

  // ---------- check ----------

  int success = 1;
  if(rank_id == 0){
    // stop time measurement
    clock_gettime(CLOCK_REALTIME, &stop);
    double result = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    printf("Final:\t\t");
    printTemperature(A, N);
    printf("\n");

    for (long long i = 0; i < N; i++) {
      double temp = A[i];
      if (273 <= temp && temp <= 273 + 60)
        continue;
      success = 0;
      break;
    }

    printf("Verification: %s\n", (success) ? "OK" : "FAILED");

   
    printf("time for computation: %f\n", result);
  }

  // ---------- cleanup ----------

  releaseVector(A);

  MPI_Finalize();

  // done
  return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Vector createVector(int N) {
  // create data and index vector
  return malloc(sizeof(double) * N);
}

void releaseVector(Vector m) { free(m); }

void printTemperature(Vector m, int N) {
  const char *colors = " .-:=+^X#%@";
  const int numColors = 12;

  // boundaries for temperature (for simplicity hard-coded)
  const double max = 273 + 30;
  const double min = 273 + 0;

  // set the 'render' resolution
  int W = RESOLUTION;

  // step size in each dimension
  int sW = N / W;

  // room
  // left wall
  printf("X");
  // actual room
  for (int i = 0; i < W; i++) {
    // get max temperature in this tile
    double max_t = 0;
    for (int x = sW * i; x < sW * i + sW; x++) {
      max_t = (max_t < m[x]) ? m[x] : max_t;
    }
    double temp = max_t;

    // pick the 'color'
    int c = ((temp - min) / (max - min)) * numColors;
    c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

    // print the average temperature
    printf("%c", colors[c]);
  }
  // right wall
  printf("X");
}