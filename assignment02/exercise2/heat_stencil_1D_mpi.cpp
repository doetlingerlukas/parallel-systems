#include <iostream>
#include <vector>
#include <mpi.h>

#define RESOLUTION 120

using namespace std;

void printTemperature(vector<double> m, int N);

int main(int argc, char **argv) {
  // problem size
  auto N = 2000;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 500;
  cout << "Computing heat-distribution for room size N=" << N << " for T=" << T << " timesteps." << endl;

  // MPI init
  int rank_id, number_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_ranks);

  // initial temp fileds
  vector<double> A (N);

  for (auto i = 0; i < N; i++) {
    A[i] = 273; // temperature is 0° C everywhere (273 K)
  }

  // heat source
  int source_x = N / 4;
  A[source_x] = 273 + 60;

  cout << "Initial:\t"; 
  printTemperature(A, N);
  cout << endl;

  // split problem size among ranks
  int N_rank = N / number_of_ranks;
  int from = (rank_id * N_rank);
  int to = (rank_id + 1) * N_rank;

  // ---------- compute ----------

  for (auto t = 0; t < T; t++) {
    for (auto i = from; i < to; i++) {
      
      if (i == source_x) {
        continue;
      }
      auto t_current = A[i];

      auto t_left = (i != 0) ? A[i - 1] : t_current;
      auto t_right = (i != N - 1) ? A[i + 1] : t_current;

      A[i] = t_current + 0.2 * (t_left + t_right + (-2 * t_current));
    }

    vector<double> to_send (A.begin() + from, A.begin() + to);
    MPI_Gather(to_send.data(), N_rank, MPI_DOUBLE, A.data(), N_rank, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank_id == 0) {
      
      if (!(t % 10000)) {
        cout << "Step t= " << t << "\t";
        printTemperature(A, N);
        cout << endl;
        cout << "Size of A: " << A.size() << endl;
      }
    }

    MPI_Bcast(A.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  // ---------- check ----------

  cout << "Final:\t\t";
  printTemperature(A, N);
  cout << endl;

  auto success = 1;
  for (auto i = 0; i < N; i++) {
    auto temp = A[i];
    if (273 <= temp && temp <= 273 + 60)
      continue;
    success = 0;
    break;
  }

  cout << "Verification: " << ((success) ? "OK" : "FAILED") << endl;

  MPI_Finalize();
  return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void printTemperature(vector<double> m, int N) {
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  // boundaries for temperature (for simplicity hard-coded)
  const double max = 273 + 30;
  const double min = 273 + 0;

  // set the 'render' resolution
  int W = RESOLUTION;

  // step size in each dimension
  int sW = N / W;

 
  cout << "|";
  
  for (auto i = 0; i < W; i++) {
    
    double max_t = 0;
    for (int x = sW * i; x < sW * i + sW; x++) {
      max_t = (max_t < m[x]) ? m[x] : max_t;
    }
    double temp = max_t;

    // pick the 'color'
    int c = ((temp - min) / (max - min)) * numColors;
    c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

    cout << colors[c];
  }

  cout << "|";
}