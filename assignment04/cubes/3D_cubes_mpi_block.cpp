#include "../../shared/stencil.hpp"

#include <cmath>
#include <chrono>
#include <mpi.h>

const static int TO_LEFT = 0;
const static int TO_RIGHT = 1;
const static int TO_UPPER = 2;
const static int TO_LOWER = 3;
const static int TO_MAIN = 4;
const static int TO_BACK = 5;
const static int TO_FRONT = 6;

using namespace std;

int main(int argc, char **argv) {

  // problem size
  auto N = 50;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto timesteps = N * 100;

  auto start_time = chrono::high_resolution_clock::now();

  int rank_id, amount_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &amount_of_ranks);

  if (fmod(cbrtf(amount_of_ranks), 1) != 0) {
    if (rank_id == 0) {
      cout <<  "Rank size must be a square number!" << endl;
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  int ranks_per_row = (int) cbrt(amount_of_ranks);

  // set up cartesian topology
  int dimensions[3] = {ranks_per_row, ranks_per_row, ranks_per_row};
	int periods[3] = {0, 0, 0};
  MPI_Comm comm_3d;
  MPI_Dims_create(amount_of_ranks, 3, dimensions); 
  MPI_Cart_create(MPI_COMM_WORLD, 3, dimensions, periods, 0, &comm_3d);

  // Neighbours rank id's.
	int left, right, upper, lower, front, back;
	MPI_Cart_shift(comm_3d, 0, 1, &left, &right);
	MPI_Cart_shift(comm_3d, 1, 1, &upper, &lower);
  MPI_Cart_shift(comm_3d, 2, 1, &front, &back);

  // Problem size for a single rank.
  auto chunk_size = N / ranks_per_row;

  // Buffers for computation.
  vector<vector<vector<double>>> buffer(chunk_size, vector<vector<double>>(chunk_size, vector<double>(chunk_size, 273)));
  vector<vector<vector<double>>> swap_buffer(chunk_size, vector<vector<double>>(chunk_size, vector<double>(chunk_size, 273)));

  // Place heat source.
  int source = N / 4;
  int source_coords[3] = {(source / chunk_size), (source / chunk_size), (source / chunk_size)};
  int source_rank;
  MPI_Cart_rank(comm_3d, source_coords, &source_rank);
  auto source_index = source % chunk_size;
  
  if (rank_id == source_rank) {
    buffer[source_index][source_index][source_index] = 273 + 60;
  }

  // Buffer to save upper/lower bordering row.
  vector<double> upper_buffer(chunk_size);
  vector<double> lower_buffer(chunk_size);
  vector<double> front_buffer(chunk_size);
  vector<double> back_buffer(chunk_size);

  // Iterate over timesteps.
  for (auto t = 0; t < timesteps; t++) { 

    // iterate over the z axis
    for (auto slice = 0; slice < chunk_size; slice++) {

      // send to lower
      if (lower >= 0) {
        MPI_Send(&buffer[slice][chunk_size-1][0], chunk_size, MPI_DOUBLE, lower, TO_LOWER, comm_3d);
      }
      // send to upper and recieve from upper
      if (upper >= 0) {
        MPI_Send(&buffer[slice][0][0], chunk_size, MPI_DOUBLE, upper, TO_UPPER, comm_3d);
        MPI_Recv(&upper_buffer[0], chunk_size, MPI_DOUBLE, upper, TO_LOWER, comm_3d, MPI_STATUS_IGNORE);
      }
      // recieve from lower
      if (lower >= 0) {
        MPI_Recv(&lower_buffer[0], chunk_size, MPI_DOUBLE, lower, TO_UPPER, comm_3d, MPI_STATUS_IGNORE);
      }

      // iterate over rows
      for (auto row = 0; row < chunk_size; row++) { 

        // send first element of current row to left neighbour
        if (left >= 0) {
          MPI_Send(&buffer[slice][row][0], 1, MPI_DOUBLE, left, TO_LEFT, comm_3d);
        }
        // send last element of current row to right neigbour
        if (right >= 0) {
          MPI_Send(&buffer[slice][row][chunk_size - 1], 1, MPI_DOUBLE, right, TO_RIGHT, comm_3d);
        }

        if ((back >= 0) && (slice == chunk_size - 1)) {
          MPI_Send(&buffer[slice][row][0], chunk_size, MPI_DOUBLE, back, TO_BACK, comm_3d);
        }

        if ((front >= 0) && (slice == 0)) {
          MPI_Send(&buffer[slice][row][0], chunk_size, MPI_DOUBLE, front, TO_FRONT, comm_3d);
          MPI_Recv(&front_buffer[0], chunk_size, MPI_DOUBLE, front, TO_BACK, comm_3d, MPI_STATUS_IGNORE);
        }

        if ((back >= 0) && (slice == chunk_size - 1)) {
          MPI_Recv(&back_buffer[0], chunk_size, MPI_DOUBLE, back, TO_FRONT, comm_3d, MPI_STATUS_IGNORE);
        }

        // iterate over columns
        for (auto column = 0; column < chunk_size; column++) {

          if (rank_id == source_rank && (row == source_index && (column == source_index && slice == source_index))) {
              swap_buffer[slice][row][column] = buffer[slice][row][column];
              continue;
          }
          
          auto current_temp = buffer[slice][row][column];
          
          auto left_temp = (column != 0) ? buffer[slice][row][column - 1] : current_temp;
          auto right_temp = (column != chunk_size - 1) ? buffer[slice][row][column + 1] : current_temp;

          auto upper_temp = (row != 0) ? buffer[slice][row - 1][column] : current_temp;
          auto lower_temp = (row != chunk_size - 1) ? buffer[slice][row + 1][column] : current_temp;

          auto front_temp = (slice != 0) ? buffer[slice - 1][row][column] : current_temp;
          auto back_temp = (slice != chunk_size - 1) ? buffer[slice + 1][row][column] : current_temp;

          if ((left >= 0) && (column == 0)) {
            MPI_Recv(&left_temp, 1, MPI_DOUBLE, left, TO_RIGHT, comm_3d, MPI_STATUS_IGNORE);
          }

          if ((right >= 0) && (column == chunk_size - 1)) { 
            MPI_Recv(&right_temp, 1, MPI_DOUBLE, right, TO_LEFT, comm_3d, MPI_STATUS_IGNORE);
          }

          if ((upper >= 0) && (row == 0)) {
            upper_temp = upper_buffer[column];
          }

          if ((lower >= 0) && (row == chunk_size - 1)) {
            lower_temp = lower_buffer[column];
          }

          if ((front >= 0) && (slice == 0)) {
            front_temp = front_buffer[column];
          }

          if ((back >= 0) && (slice == chunk_size - 1)) {
            back_temp = back_buffer[column];
          }

          swap_buffer[slice][row][column] = current_temp + 0.14 * (left_temp + right_temp + upper_temp + lower_temp + front_temp + back_temp + (-6 * current_temp));  
        }
      }
    }
    
    // swap matrices (just pointers, not content)
    buffer.swap(swap_buffer);
  }
  
  // Collect results.
  if (rank_id == 0){
    vector<vector<vector<double>>> result(N, vector<vector<double>>(N, vector<double>(N)));

    // Array to save coordinates.
    int coord[3];
    
    // Add buffers from ranks to result.
    for (auto i = 1; i < amount_of_ranks; i++){
      
      auto receive_size = chunk_size * chunk_size * chunk_size;
      vector<double> A(receive_size);
      MPI_Recv(&A[0], receive_size, MPI_DOUBLE, i, TO_MAIN, comm_3d, MPI_STATUS_IGNORE);
      
      MPI_Cart_coords(comm_3d, i, 3, coord);

      for (auto slice = 0; slice < chunk_size; slice++) {
        for (auto row = 0; row < chunk_size; row++) {
          for (auto column = 0; column < chunk_size; column++) {
            result[slice + coord[2] * chunk_size][row + coord[1] * chunk_size][column + coord[0] * chunk_size] = A[column + row * chunk_size + slice * chunk_size * chunk_size];
          }
        }
      }
    }

    // Add buffer of rank 0 to result.
    MPI_Cart_coords(comm_3d, 0, 3, coord);
    for (auto slice = 0; slice < chunk_size; slice++) {
      for (auto row = 0; row < chunk_size; row++) {
        for (auto column = 0; column < chunk_size; column++) {
          result[slice + coord[2] * chunk_size][row + coord[1] * chunk_size][column + coord[0] * chunk_size] = buffer[slice][row][column];
        }
      }
    }
    
    //printTemperature(result[0], N);
    printTemperature(result[source], N, 50, 30);
    //printTemperature(result[chunk_size], N);
    //printTemperature(result[N-1], N);

    // verification
    if (verify3d(result, N)) {
      cout << "VERIFICATION: SUCCESS!" << endl;
    } else {
      cout << "VERIFICATION: FAILURE!" << endl;
    }

    // Measure time.
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
    cout << endl;
    cout << "This took " << duration << " seconds." << endl;

  } else { 
    // Send buffer to rank 0.
    auto send_size = chunk_size * chunk_size * chunk_size;

    vector<double> to_send;
    for (auto slice = 0; slice < chunk_size; slice++) {
      for (auto row = 0; row < chunk_size; row++) {
        for (auto column = 0; column < chunk_size; column++) {
          to_send.push_back(buffer[slice][row][column]);
        }
      }
    }
    MPI_Send(&to_send[0], send_size, MPI_DOUBLE, 0, TO_MAIN, comm_3d);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}