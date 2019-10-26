#include <iostream>
#include <vector>
#include <mpi.h>

#define RESOLUTION 80

const static int TO_LEFT = 0;
const static int TO_RIGHT = 1;
const static int TO_UPPER = 2;
const static int TO_LOWER = 3;
const static int TO_MAIN = 4;

using namespace std;

void printTemperature(double **m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 48; // has to be devisable by 4
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 100;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << " for T=" << T << " timesteps." << endl;

  int rank_id, number_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_ranks);

  // set up grid
  int dims[2] = {0, 0};
	int periods[2] = {1, 1}; // to avoid too many edge cases in code
  //int mycoords[2];
  MPI_Comm comm_2d;
  MPI_Dims_create(number_ranks, 2, dims); 
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_2d);

  // define neighbors
	int left, right, upper, lower;
	MPI_Cart_shift(comm_2d, 0, 1, &left, &right);
	MPI_Cart_shift(comm_2d, 1, 1, &upper, &lower);

  // split problem size among ranks
  auto N_rank = N / number_ranks;

  // init matrix 
  double** rank_buffer = new double*[N_rank];
  double** rank_swap_buffer = new double*[N_rank];
  for (auto i = 0; i<N_rank; i++){
    rank_buffer[i] = new double[N_rank];
    rank_swap_buffer[i] = new double[N_rank];
    for (auto j = 0; j<N_rank; j++){
      rank_buffer[i][j] = 273.0;
      rank_swap_buffer[i][j] = 273.0;
    }
  }

  auto source_x = N/4;
  auto source_y = N/4;
  bool contains_source = rank_id == (source_x / N_rank) && (source_y / N_rank);
  auto source_index_x = source_x % N_rank;
  auto source_index_y = source_y % N_rank;

  if (contains_source) {
    rank_buffer[source_index_x][source_index_y] = 273 + 60;
  }

  for (auto t = 0; t < T; t++) { // iterate timesteps
    for (auto i = 0; i < N_rank; i++) { // iterate rows
      for (auto j = 0; j < N_rank; j++) { // iterate columns
        
        auto t_current = rank_buffer[i][j];
        auto t_upper = (i != 0) ? rank_buffer[i - 1][j] : t_current;
        auto t_lower = (i != N_rank - 1) ? rank_buffer[i + 1][j] : t_current;
        auto t_left = (j != 0) ? rank_buffer[i][j - 1] : t_current;
        auto t_right = (j != N_rank - 1) ? rank_buffer[i][j + 1] : t_current;

        if (j+1 == N_rank){ // send/rec right
          MPI_Request req;
          MPI_Isend(&rank_buffer[i][N_rank-1], 1, MPI_DOUBLE, right, TO_RIGHT, comm_2d, &req);
          MPI_Request_free(&req); 

          MPI_Recv(&t_right, 1, MPI_DOUBLE, right, TO_RIGHT, comm_2d, MPI_STATUS_IGNORE);
        }

        if (j == 0){ // send/rec left
          MPI_Request req;
          MPI_Isend(&rank_buffer[i][0], 1, MPI_DOUBLE, left, TO_LEFT, comm_2d, &req);
          MPI_Request_free(&req); 

          MPI_Recv(&t_left, 1, MPI_DOUBLE, left, TO_LEFT, comm_2d, MPI_STATUS_IGNORE);
        }

        if (i+1 == N_rank){ // send/rec lower
          MPI_Request req;
          MPI_Isend(&rank_buffer[i][j], 1, MPI_DOUBLE, lower, TO_LOWER, comm_2d, &req);
          MPI_Request_free(&req);

          MPI_Recv(&t_lower, 1, MPI_DOUBLE, lower, TO_LOWER, comm_2d, MPI_STATUS_IGNORE);
        }
        if (i == 0){// send/rec upper
          MPI_Request req;
          MPI_Isend(&rank_buffer[i][j], 1, MPI_DOUBLE, upper, TO_UPPER, comm_2d, &req);
          MPI_Request_free(&req);

          MPI_Recv(&t_upper, 1, MPI_DOUBLE, upper, TO_UPPER, comm_2d, MPI_STATUS_IGNORE);
        }

        if (i == source_x && j == source_y) {
            rank_swap_buffer[i][j] = rank_buffer[i][j];
            continue;
        }

        rank_swap_buffer[i][j] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + (-4 * t_current));  
      }
    }
    
    // swap matrices (just pointers, not content)
    double **swap = rank_buffer;
    rank_buffer = rank_swap_buffer;
    rank_swap_buffer = swap;
    
  }

  // result array
  double** A = new double*[N];
  for (auto i = 0; i < N; i++){
    A[i] = new double[N];
    for (auto j = 0; j < N; j++){
      A[i][j] = 0.0;
    }
  }

  // gather array on rank 0
  MPI_Gather(rank_buffer, N_rank * N_rank, MPI_DOUBLE, A, N * N, MPI_DOUBLE, 0, comm_2d);

  // print final result
  //printTemperature(A, N);

  MPI_Finalize();
  return EXIT_SUCCESS;
}


void printTemperature(double **m, int N) {
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  // boundaries for temperature (for simplicity hard-coded)
  const double max = 273 + 30;
  const double min = 273 + 0;

  // set the 'render' resolution
  int W = RESOLUTION;
  int H = RESOLUTION;
  if (N < RESOLUTION){
    W = N;
    H = N;
  }


  // step size in each dimension
  int sW = N / W;
  int sH = N / H;
 
  // top wall
  for (auto i = 0; i < W + 2; i++) {
    cout << "-";
  }
  cout << endl;
  
  for (auto i = 0; i < H; i++){
    // left wall
    cout << "|";
    
    // computing a row
    for (auto j = 0; j < W; j++) {
      
      double max_t = 0;
      for (auto x = sH * i; x < sH * i + sH; x++) {
        for (auto y = sW * j; y < sW * j + sW; y++) {
          max_t = (max_t < m[x][y]) ? m[x][y] : max_t;
        }
      }
      double temp = max_t;

      // pick the 'color'
      int c = ((temp - min) / (max - min)) * numColors;
      c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);
      cout << colors[c];
    }
    // right wall
    cout << "|" << endl;
  }

  // bottom wall
  for (auto i = 0; i < W + 2; i++) {
    cout << "-";
  }
  cout << endl;
}
