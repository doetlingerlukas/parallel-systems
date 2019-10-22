#include <iostream>
#include <vector>
#include <mpi.h>

#define RESOLUTION 120

using namespace std;

// Tgs for MPI communiction
const static int TO_LEFT = 0;
const static int TO_RIGHT = 1;
const static int TO_MAIN = 2;

void printTemperature(vector<double> m, int N);

int main(int argc, char **argv) {
  
  auto N = 1000;
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
  auto N_rank = N / number_of_ranks;
  auto N_last_rank = N - (N_rank * (number_of_ranks - 1));
  auto buffer_size = rank_id == number_of_ranks - 1 ? N_last_rank : N_rank;

  // compuation buffer for each rank
  vector<double> rank_buffer (buffer_size, 273);
  vector<double> rank_swap_buffer (buffer_size, 273);

  // heat source
  auto source = N / 4;
  bool contains_source = rank_id == (source / N_rank);
  auto source_index = source % N_rank;

  if (contains_source) {
    rank_buffer[source_index] = 273 + 60;
  }

  // loop over time
  for (auto t = 0; t < T; t++) {
    
    // send first element of buffer to left neighbour
    if (rank_id > 0) {
      MPI_Request req;
      MPI_Isend(&rank_buffer[0], 1, MPI_DOUBLE, rank_id - 1, TO_LEFT, MPI_COMM_WORLD, &req);
      MPI_Request_free(&req);
    }
    // send last element of buffer to right neigbour
    if (rank_id < (number_of_ranks - 1)) {
      MPI_Request req;
      MPI_Isend(&rank_buffer[N_rank - 1], 1, MPI_DOUBLE, rank_id + 1, TO_RIGHT, MPI_COMM_WORLD, &req);
      MPI_Request_free(&req);
    }

    // loop over buffer
    for (auto i = 0; i < buffer_size; i++) {
      
      if (contains_source && source_index == i) {
        rank_swap_buffer[i] = rank_buffer[i];
        continue;
      }
      auto current_temp = rank_buffer[i];
      double left_temp;
      double right_temp;

      // calclate left temp
      if (i == 0) {
        if (rank_id == 0) {
          left_temp = current_temp;
        } else {
          MPI_Recv(&left_temp, 1, MPI_DOUBLE, rank_id - 1, TO_RIGHT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      } else {
        left_temp = rank_buffer[i - 1];
      }
      // calculate right temp
      if (i == buffer_size - 1) {
        if (rank_id == number_of_ranks - 1) {
          right_temp = current_temp;
        } else {
          MPI_Recv(&right_temp, 1, MPI_DOUBLE, rank_id + 1, TO_LEFT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      } else {
        right_temp = rank_buffer[i + 1];
      }

      rank_swap_buffer[i] = current_temp + 0.2 * (left_temp + right_temp + (-2 * current_temp));
    }

    // swap pointers of buffer
    swap(rank_buffer, rank_swap_buffer);
  }

  // Send all results to rank 0
  if (rank_id == 0) {
    rank_buffer.resize(N);

    for (auto i=1; i < number_of_ranks; i++) {
      MPI_Recv(&rank_buffer[0] + N_rank * i, (i == number_of_ranks - 1 ? N_last_rank : N_rank), MPI_DOUBLE, i, TO_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  } else {
    MPI_Send(&rank_buffer[0], buffer_size, MPI_DOUBLE, 0, TO_MAIN, MPI_COMM_WORLD);
  }

  // ---------- check ----------  
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
    for (auto x = sW * i; x < sW * i + sW; x++) {
      max_t = (max_t < m[x]) ? m[x] : max_t;
    }
    double temp = max_t;

    int c = ((temp - min) / (max - min)) * numColors;
    c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

    cout << colors[c];
  }
  cout << "|";
}
