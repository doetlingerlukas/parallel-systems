#include <iostream>
#include <vector>
#include <mpi.h>

#define RESOLUTION 120

using namespace std;

void printTemperature(vector<double> m, int N);

int main(int argc, char **argv) {
  // problem size
  auto N = 200;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }

  // MPI init
  int rank_id, number_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_ranks);

  double start = MPI_Wtime();

  auto T = N * 500;
  if (rank_id == 0) {
    cout << "Computing heat-distribution for room size N=" << N << " for T=" << T << " timesteps." << endl;
  }

  // split problem size among ranks
  int N_rank = (N + (number_of_ranks - 1)) / number_of_ranks;
  int N_last_rank = N - ((N - N_rank) / N_rank) * N_rank;
  auto buffer_size = rank_id == number_of_ranks - 1 ? N_last_rank : N_rank;

  // compuation buffer for each rank
  vector<double> rank_buffer (buffer_size, 273);
  vector<double> rank_swap_buffer (buffer_size, 273);

  // heat source
  int source_x = N / 4;
  bool contains_source = rank_id == (source_x / N_rank);
  int source_index = source_x % N_rank;

  if (contains_source) {
    rank_buffer[source_index] = 273 + 60;
  }

  // ---------- compute ----------
  for (auto t = 0; t < T; t++) {
    
    if (rank_id > 0) {
      MPI_Request req;
      MPI_Isend(&rank_buffer[0], 1, MPI_DOUBLE, rank_id - 1, 0, MPI_COMM_WORLD, &req);
      // cout << "Sent from " << rank_id << " to " << right << endl;
      MPI_Request_free(&req);
    }
    if (rank_id < (number_of_ranks - 1)) {
      MPI_Request req;
      MPI_Isend(&rank_buffer[N_rank - 1], 1, MPI_DOUBLE, rank_id + 1, 1, MPI_COMM_WORLD, &req);
      // cout << "Sent from " << rank_id << " to " << left << endl;
      MPI_Request_free(&req);
    }

    for (auto i = 0; i < buffer_size; i++) {
      
      if (contains_source && source_index == i) {
        rank_swap_buffer[i] = rank_buffer[i];
        continue;
      }
      auto t_current = rank_buffer[i];

      double t_left;
      double t_right;

      if (i == 0) {
        if (rank_id == 0) {
          t_left = t_current;
        } else {
          MPI_Recv(&t_left, 1, MPI_DOUBLE, rank_id - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      } else {
        t_left = rank_buffer[i - 1];
      }

      if (i == buffer_size - 1) {
        if (rank_id == number_of_ranks - 1) {
          t_right = t_current;
        } else {
          MPI_Recv(&t_right, 1, MPI_DOUBLE, rank_id + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      } else {
        t_right = rank_buffer[i + 1];
      }

      rank_swap_buffer[i] = t_current + 0.2 * (t_left + t_right + (-2 * t_current));
    }

    // swap buffer
    swap(rank_buffer, rank_swap_buffer);
  }

  if (rank_id == 0) {
    rank_buffer.resize(N);

    for (auto i=1; i < number_of_ranks; i++) {
      MPI_Recv(&rank_buffer[0] + N_rank * i, (i == number_of_ranks - 1 ? N_last_rank : N_rank), MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  } else {
    MPI_Send(&rank_buffer[0], buffer_size, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  // ---------- check ----------
  /*
  vector<double> A;
  MPI_Gather(rank_buffer.data(), N_rank, MPI_DOUBLE, A.data(), N_rank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  */
  
  if (rank_id == 0) {
    cout << "Final:\t\t";
    printTemperature(rank_buffer, N);
    cout << endl;
    
    auto success = 1;
    for (auto i = 0; i < N; i++) {
      auto temp = rank_buffer[i];
      if (273 <= temp && temp <= 273 + 60)
        continue;
      success = 0;
      break;
    }

    cout << "Verification: " << ((success) ? "OK" : "FAILED") << endl;
    cout << "elapsed time: " << MPI_Wtime() - start << endl;
  }
  
  MPI_Finalize();
  return EXIT_SUCCESS;
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
