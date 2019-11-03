#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <mpi.h>

#define RESOLUTION 80

const static int TO_UPPER = 1;
const static int TO_LOWER = 2;
const static int TO_MAIN = 3;

using namespace std;

void printTemperature(vector<vector<vector<double>>> m, int N, int h);

bool verify3d(vector<vector<vector<double>>> m, int problem_size);

int main(int argc, char **argv) {

  // problem size
  auto N = 32;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto timesteps = N * 50;

  auto start_time = chrono::high_resolution_clock::now();

  int rank_id, amount_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &amount_of_ranks);

  // set up cartesian topology
  int dimensions[1] = {amount_of_ranks}; //slabs
	int periods[1] = {0};
  MPI_Comm comm_3d;
  MPI_Dims_create(amount_of_ranks, 1, dimensions); 
  MPI_Cart_create(MPI_COMM_WORLD, 1, dimensions, periods, 0, &comm_3d);

  // Neighbours rank id's.
	int upper, lower;
	MPI_Cart_shift(comm_3d, 1, 1, &upper, &lower);

  cout << "rank: " << rank_id << "| upper: " << upper << "| lower: " << lower << endl;

  // Problem size on z axis for a single rank.
  int slabs_per_rank = (int) N / amount_of_ranks;
  
  // Buffers for computation.
  vector<vector<vector<double>>> buffer(N, vector<vector<double>>(slabs_per_rank, vector<double>(N, 273)));
  vector<vector<vector<double>>> swap_buffer(N, vector<vector<double>>(slabs_per_rank, vector<double>(N, 273)));
  
  // Place heat source.
  int source = N / 4;
  int source_coords[3] = {(source / N), (source / slabs_per_rank), (source / N)};
  int source_rank;
  MPI_Cart_rank(comm_3d, source_coords, &source_rank);
  auto source_index = source % N;
  auto source_index_slab = source % slabs_per_rank;
  
  if (rank_id == source_rank) {
    buffer[source_index][source_index_slab][source_index] = 273 + 60;
  }
  
  // Buffer to save upper/lower bordering row.
  vector<double> upper_buffer(N);
  vector<double> lower_buffer(N);

  MPI_Request req;

  // Iterate over timesteps.
  for (auto t = 0; t < timesteps; t++) { 

    // iterate over the z axis
    for (auto slice = 0; slice < N; slice++) {

      // send/receive lower
      if (lower >= 0) {
        MPI_Isend(&buffer[slice][slabs_per_rank-1][0], N, MPI_DOUBLE, lower, TO_LOWER, comm_3d, &req);
        MPI_Irecv(&lower_buffer[0], N, MPI_DOUBLE, lower, TO_UPPER, comm_3d, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
      }
      // send/receive upper
      if (upper >= 0) {
        MPI_Isend(&buffer[slice][0][0], N, MPI_DOUBLE, upper, TO_UPPER, comm_3d, &req);
        MPI_Irecv(&upper_buffer[0], N, MPI_DOUBLE, upper, TO_LOWER, comm_3d, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
      }

      // iterate over rows
      for (auto row = 0; row < slabs_per_rank; row++) { 

        // iterate over columns
        for (auto column = 0; column < N; column++) {
          
          if (rank_id == source_rank && (row == source_index_slab && (column == source_index && slice == source_index))) {
              swap_buffer[slice][row][column] = buffer[slice][row][column];
              continue;
          }
          
          auto current_temp = buffer[slice][row][column];
          
          auto left_temp = (column != 0) ? buffer[slice][row][column - 1] : current_temp;
          auto right_temp = (column != N - 1) ? buffer[slice][row][column + 1] : current_temp;

          auto upper_temp = (row != 0) ? buffer[slice][row - 1][column] : current_temp;
          auto lower_temp = (row != slabs_per_rank - 1) ? buffer[slice][row + 1][column] : current_temp;
          
          auto front_temp = (slice != 0) ? buffer[slice - 1][row][column] : current_temp;
          auto back_temp = (slice != N - 1) ? buffer[slice + 1][row][column] : current_temp;

          
          if ((upper >= 0) && (row = slabs_per_rank - 1) && rank_id != source_rank) {
            if(upper_buffer[column] > 274){
              //cout << upper_buffer[column] << endl;
              upper_temp = upper_buffer[column];
            }
          }
          
          if ((lower >= 0) && (row == 0) && rank_id != source_rank) {
            if(lower_buffer[column] > 273){
              //cout << lower_buffer[column] << endl;
              lower_temp = lower_buffer[column];
            }
          }
          
          buffer[slice][row][column] = current_temp + 0.14 * (left_temp + right_temp + upper_temp + lower_temp + front_temp + back_temp + (-6 * current_temp));
        }
      }
    }
  }
  
  if(rank_id==source_rank){
    cout << "-----------------------------" << endl;
    cout << "source rank id " << rank_id << endl;
    printTemperature(buffer, N, source_index_slab);
    cout << "-----------------------------" << endl;
  }else{
    cout << "-----------------------------" << endl;
    cout << "rank id " << rank_id << endl;
    printTemperature(buffer, N, slabs_per_rank-1);
    cout << "-----------------------------" << endl;
  }
  
  // Collect results nd verify.
  if (rank_id == 0){
    vector<vector<vector<double>>> result(N, vector<vector<double>>(N, vector<double>(N)));

    // Array to save coordinates.
    int coord[1];
    
    // Add buffers from ranks to result.
    for (auto i = 1; i < amount_of_ranks; i++){
      
      auto receive_size = N * slabs_per_rank * N;
      vector<double> A(receive_size);
      MPI_Recv(&A[0], receive_size, MPI_DOUBLE, i, TO_MAIN, comm_3d, MPI_STATUS_IGNORE);
      
      MPI_Cart_coords(comm_3d, i, 1, coord);
      for (auto slice = 0; slice < N; slice++) {
        for (auto row = 0; row < slabs_per_rank; row++) {
          for (auto column = 0; column < N; column++) {
            result[slice][row + coord[0] * slabs_per_rank][column] = A[column + row * slabs_per_rank + slice * slabs_per_rank * N];
          }
        }
      }
    }
    
    // Add buffer of rank 0 to result.
    MPI_Cart_coords(comm_3d, 0, 1, coord);
    for (auto slice = 0; slice < N; slice++) {
      for (auto row = 0; row < slabs_per_rank; row++) {
        for (auto column = 0; column < N; column++) {
          result[slice][row + coord[0] * slabs_per_rank][column] = buffer[slice][row][column];
        }
      }
    }

    if (verify3d(result, N)) {
      cout << "VERIFICATION: SUCCESS!" << endl;
    } else {
      cout << "VERIFICATION: FAILURE!" << endl;
    }

    // Measure time.
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << endl;
    cout << "This took " << duration << " milliseconds." << endl;

  } else { 
    // Send buffer to rank 0.
    auto send_size = N * slabs_per_rank * N;

    vector<double> to_send;
    for (auto slice = 0; slice < N; slice++) {
      for (auto row = 0; row < slabs_per_rank; row++) {
        for (auto column = 0; column < N; column++) {
          to_send.push_back(buffer[slice][row][column]);
        }
      }
    }
    MPI_Send(&to_send[0], send_size, MPI_DOUBLE, 0, TO_MAIN, comm_3d);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}


void printTemperature(vector<vector<vector<double>>> m, int N, int h) {
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
          max_t = (max_t < m[x][h][y]) ? m[x][h][y] : max_t;
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

bool verify3d(vector<vector<vector<double>>> m, int problem_size) {
  bool result = true;

  for (auto slice = 0; slice < problem_size; slice++) {
    for (auto row = 0; row < problem_size; row++) {
      for (auto column = 0; column < problem_size; column++){
        auto temp = m[slice][row][column];

        if (temp < 273.0 || temp > 273.0 + 60.0) {
          //cout << temp << endl;
          result = false;
        }
      }
    }
  }

  return result;
}