#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>
#include <cmath>

#define RESOLUTION 80

using namespace std;

const int TO_LOWER = 0;
const int TO_UPPER = 1;
const int TO_LEFT = 2;
const int TO_RIGHT = 3;
const int TO_MAIN = 4;

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

  if (fmod(sqrtf(amount_of_ranks), 1) != 0) {
    if (rank_id == 0) {
      cout <<  "Rank size must be a square number!" << endl;
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  int ranks_per_row = sqrt(amount_of_ranks);

  // set up cartesian topology
  int dimensions[2] = {ranks_per_row, ranks_per_row};
  int periods[2] = {0, 0};
  int ndims = 2;
  MPI_Comm comm_2d;
  MPI_Dims_create(amount_of_ranks, ndims, dimensions); 
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dimensions, periods, 0, &comm_2d);

  // Neighbours rank id's.
  int upper, lower, left, right;
  MPI_Cart_shift(comm_2d, 0, 1, &left, &right);
  MPI_Cart_shift(comm_2d, 1, 1, &upper, &lower);

  // Problem size for a single rank.
  auto chunk_size = N / amount_of_ranks;

  // Buffers for computation.
  vector<vector<vector<double>>> buffer(N, vector<vector<double>>(chunk_size, vector<double>(chunk_size, 273)));
  vector<vector<vector<double>>> swap_buffer(N, vector<vector<double>>(chunk_size, vector<double>(chunk_size, 273)));

  // Place heat source.
  int source_y = chunk_size / 2;
  int source_x = chunk_size / 2;
  int source_z = N / 4;
  if (rank_id == 0) {
    buffer[source_z][source_x][source_y] = 273 + 60;
  }

  // Buffer to save upper/lower border row.
  vector<double> left_buffer(N);
  vector<double> right_buffer(N);
  vector<double> upper_buffer(N);
  vector<double> lower_buffer(N);

  for (auto t = 0; t < timesteps; t++) { // iterate timesteps
    for (auto i = 0; i < N; i++) { // slices

      // send/rec upper/lower row
      if (lower >= 0) { // lower
        MPI_Send(&buffer[i][chunk_size-1], chunk_size, MPI_DOUBLE, lower, TO_LOWER, comm_2d);
        MPI_Recv(&lower_buffer[0], chunk_size, MPI_DOUBLE, lower, TO_UPPER, comm_2d, MPI_STATUS_IGNORE);
      }

      if (upper >= 0) { // upper
        MPI_Send(&buffer[i][0], chunk_size, MPI_DOUBLE, upper, TO_UPPER, comm_2d);
        MPI_Recv(&upper_buffer[0], chunk_size, MPI_DOUBLE, upper, TO_LOWER, comm_2d, MPI_STATUS_IGNORE);
      }

      for (auto j = 0; j < chunk_size; j++) { // rows

        // send 0 and chunk_size-1 element to left/right
        if (left >= 0) {
          MPI_Send(&buffer[i][j][0], 1, MPI_DOUBLE, left, TO_LEFT, comm_2d);
        }
        // send last element of current row to right neigbour
        if (right >= 0) {
          MPI_Send(&buffer[i][j][chunk_size - 1], 1, MPI_DOUBLE, right, TO_RIGHT, comm_2d);
        }

        for (auto k = 0; k < chunk_size; k++){ // columns
          if (rank_id == 0 && (i == source_z && j == source_x && k == source_y)) {
              swap_buffer[i][j][k] = buffer[i][j][k];
              continue;
          }
                     
          auto t_current = buffer[i][j][k];

          auto t_upper = (i != 0) ? buffer[i - 1][j][k] : t_current;
          auto t_lower = (i != chunk_size - 1) ? buffer[i + 1][j][k] : t_current;
          auto t_left = (j != 0) ? buffer[i][j - 1][k] : t_current;
          auto t_right = (j != chunk_size - 1) ? buffer[i][j + 1][k] : t_current;
          auto t_before = (k != 0) ? buffer[i][j][k - 1] : t_current;
          auto t_behind = (k != N - 1) ? buffer[i][j][k + 1] : t_current;

          if ((left >= 0) && (j == 0)) {
            MPI_Recv(&t_left, 1, MPI_DOUBLE, left, TO_RIGHT, comm_2d, MPI_STATUS_IGNORE);
          }

          if ((right >= 0) && (j == chunk_size - 1)) { 
            MPI_Recv(&t_right, 1, MPI_DOUBLE, right, TO_LEFT, comm_2d, MPI_STATUS_IGNORE);
          }

          buffer[i][j][k] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + t_before + t_behind + (6 * t_current));
        }
        swap(buffer, swap_buffer);
      }
    }

    // swap matrices (just pointers, not content)
    swap(buffer, swap_buffer);
  }
  
  auto send_size = N * chunk_size * chunk_size;

  // Collect results.
  if (rank_id == 0){

    vector<vector<vector<double>>> result(N, vector<vector<double>>(N, vector<double>(N)));
    
    // Add buffers from ranks to result.
    for (auto l = 1; l < amount_of_ranks; l++){
    
      vector<double> rank_result(send_size);
      MPI_Recv(&rank_result[0], send_size, MPI_DOUBLE, l, TO_MAIN, comm_2d, MPI_STATUS_IGNORE);

      for (auto i = 0; i < N; i++) {
        for (auto j = 0; j < chunk_size; j++) {
          for (auto k = 0; k < chunk_size; k++) {
            result[k + l * chunk_size][j + l*chunk_size][i] = rank_result[k + j*chunk_size + i*N];
          }
        }
      }
    }

    // Add buffer of rank 0 to result.
    for (auto i = 0; i < N; i++) {
      for (auto j = 0; j < chunk_size; j++) {
        for (auto k = 0; k < chunk_size; k++) {
          result[k][j][i] = buffer[i][j][k];
        }
      }
    }
    
    //printTemperature(result[0], N);
    printTemperature(result[source_z], N);

  } else { // send buffer to rank 0
    vector<double> to_send;
    for (auto i = 0; i < N; i++){
      for (auto j = 0; j < chunk_size; j++){
        for (auto k = 0; k < chunk_size; k++){
          to_send.push_back(buffer[i][j][k]);
        }
      }
    }
    MPI_Send(&to_send[0], send_size, MPI_DOUBLE, 0, TO_MAIN, comm_2d);
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
