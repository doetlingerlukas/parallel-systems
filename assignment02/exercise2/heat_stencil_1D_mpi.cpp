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

  /*
  cout << "Initial:\t"; 
  printTemperature(A, N);
  cout << endl;*/

  // split problem size among ranks
  int N_rank = (N + (number_of_ranks - 1)) / number_of_ranks;
  int from = (rank_id * N_rank);
  int to = (rank_id + 1) * N_rank;

  // determine neighbour ranks ( -1 stands for no rank )
  int left = rank_id - 1;
  int right = rank_id == (number_of_ranks - 1) ? -1 : rank_id - 1;
  double left_cache = 0;
  double right_cache = 0;

  // compuation buffer for each rank
  vector<double> rank_buffer (N_rank, 273);
  vector<double> rank_buffer_b (N_rank, 273);

  // heat source
  int source_x = N / 4;
  bool contains_source = rank_id == (source_x / N_rank);

  if (contains_source) {
    int source_index = source_x % N_rank;
    rank_buffer[source_index] = 273 + 60;
  }

  // ---------- compute ----------
  for (auto t = 0; t < T; t++) {
    for (auto i = 0; i < rank_buffer.size(); i++) {
      
      if (contains_source && (source_x % N_rank) == i) {
        continue;
      }
      auto t_current = rank_buffer[i];

      double t_left;
      double t_right;

      if (right >= 0 && i == (rank_buffer.size() - 1)) {
        MPI_Request req;
        MPI_Isend(&t_current, 1, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, &req);
        cout << "Sent from " << rank_id << " to " << right << endl;
        MPI_Request_free(&req);
      }

      if (i == 0) {
        if (rank_id == 0) {
          t_left = t_current;
        } else {
          MPI_Request req;
          MPI_Isend(&t_current, 1, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, &req);
          cout << "Sent from " << rank_id << " to " << left << endl;
          MPI_Request_free(&req);
          MPI_Recv(&left_cache, 1, MPI_DOUBLE, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          t_left = left_cache;
        }
      } else {
        t_left = rank_buffer[i - 1];
      }

      if (right >= 0 && i == (rank_buffer.size() - 1)) {
        MPI_Recv(&right_cache, 1, MPI_DOUBLE, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        t_right = right_cache;
      } else {
        t_right = (right >= 0) ? rank_buffer[i + 1] : t_current;
      }

      rank_buffer_b[i] = t_current + 0.2 * (t_left + t_right + (-2 * t_current));
    }

    // swap buffer content
    swap(rank_buffer, rank_buffer_b);

    if (rank_id == 0) {
      /*
      if (!(t % 10000)) {
        cout << "Step t= " << t << "\t";
        printTemperature(A, N);
        cout << endl;
        cout << "Size of A: " << A.size() << endl;
      }*/
    }
  }

  // ---------- check ----------

  vector<double> A;
  MPI_Gather(rank_buffer.data(), N_rank, MPI_DOUBLE, A.data(), N_rank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  if (rank_id == 0) {
    cout << "Final:\t\t";
    printTemperature(A, N);
    cout << endl;
  }

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