#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>

#define RESOLUTION 80

using namespace std;

const int TO_LOWER = 0;
const int TO_UPPER = 1;
const int TO_MAIN = 2;

void printTemperature(vector<vector<double>> m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 50; // has to be devisable by 4
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto timesteps = N*100;

  int rank_id, amount_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &amount_of_ranks);

  // set up cartesian topology
  int dimensions[2] = {amount_of_ranks};
	int periods[2] = {0};
  int ndims = 1;
  MPI_Comm comm_2d;
  MPI_Dims_create(amount_of_ranks, ndims, dimensions); 
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dimensions, periods, 0, &comm_2d);

  // Neighbours rank id's.
  int upper, lower;
  MPI_Cart_shift(comm_2d, 0, 1, &upper, &lower);

  // Problem size for a single rank.
  auto chunk_height = N / amount_of_ranks;

  // Buffers for computation.
  vector<vector<double>> buffer(chunk_height, vector<double>(N, 273));
  vector<vector<double>> swap_buffer(chunk_height, vector<double>(N, 273));

  // Place heat source.
  int source_y = N / 4;
  int source_x = chunk_height / 2;
  if (rank_id == 0) {
    buffer[source_x][source_y] = 273 + 60;
  }

  // Buffer to save upper/lower border row.
  vector<double> upper_buffer(N);
  vector<double> lower_buffer(N);

  for (auto t = 0; t < timesteps; t++) { // iterate timesteps

    if (lower >= 0) { // lower
      MPI_Send(&buffer[chunk_height-1][0], N, MPI_DOUBLE, lower, TO_LOWER, comm_2d);
      MPI_Recv(&lower_buffer[0], N, MPI_DOUBLE, lower, TO_UPPER, comm_2d, MPI_STATUS_IGNORE);
    }

    if (upper >= 0) { // upper
      MPI_Send(&buffer[0][0], N, MPI_DOUBLE, upper, TO_UPPER, comm_2d);
      MPI_Recv(&upper_buffer[0], N, MPI_DOUBLE, upper, TO_LOWER, comm_2d, MPI_STATUS_IGNORE);
    }

    for (auto i = 0; i < chunk_height; i++) { // iterate rows
      for (auto j = 0; j < N; j++) { // iterate columns

        if (rank_id == 0 && (i == source_x && j == source_y)) {
            swap_buffer[i][j] = buffer[i][j];
            continue;
        }

        auto t_current = buffer[i][j];  
        auto t_left = (j != 0) ? buffer[i][j - 1] : t_current;
        auto t_right = (j != N - 1) ? buffer[i][j + 1] : t_current;

        auto t_upper = (i != 0) ? buffer[i - 1][j] : t_current;
        if ((upper >= 0) && (i == 0)) {
          t_upper = upper_buffer[j];
        }

        auto t_lower = (i != chunk_height - 1) ? buffer[i + 1][j] : t_current;
        if ((lower >= 0) && (i == chunk_height - 1)) {
          t_lower = lower_buffer[j];
        }

        swap_buffer[i][j] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + (-4 * t_current));
      }
    }

    // swap matrices (just pointers, not content)
    swap(buffer, swap_buffer);
  }

  auto buffer_size = chunk_height * N;

  // Collect results.
  if (rank_id == 0){
    
    vector<vector<double>> result(N, vector<double>(N));
   
    // Add buffers from ranks to result.
    for (auto i = 1; i < amount_of_ranks; i++){

      vector<double> A(buffer_size);
      MPI_Recv(&A[0], buffer_size, MPI_DOUBLE, i, TO_MAIN, comm_2d, MPI_STATUS_IGNORE);

      for (auto j = 0; j < chunk_height; j++) {
        for (auto k = 0; k < N; k++) {
          result[j + i * chunk_height][k] = A[k + j * N];
        }
      }
    }

    // Add buffer of rank 0 to result.
    for (auto i = 0; i < chunk_height; i++) {
      for (auto j = 0; j < N; j++) {
        result[i][j] = buffer[i][j];
      }
    }

    printTemperature(result, N);

  } else { // Send buffer to rank 0
    vector<double> to_send;
    for (auto i = 0; i < chunk_height; i++) {
      for (auto j = 0; j < N; j++) {
        to_send.push_back(buffer[i][j]);
      }
    }
    MPI_Send(&to_send[0], buffer_size, MPI_DOUBLE, 0, TO_MAIN, comm_2d);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}


void printTemperature(vector<vector<double>> m, int N) {
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
