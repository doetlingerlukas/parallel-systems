#include "../shared/stencil.hpp"

#include <cmath>
#include <chrono>
#include <mpi.h>

const static int TO_LEFT = 0;
const static int TO_RIGHT = 1;
const static int TO_UPPER = 2;
const static int TO_LOWER = 3;
const static int TO_MAIN = 4;

using namespace std;

int main(int argc, char **argv) {

  // problem size
  auto N = 400; // has to be devisable by 4
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto timesteps = N * 100;

  auto start_time = chrono::high_resolution_clock::now();

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
  MPI_Comm comm_2d;
  MPI_Dims_create(amount_of_ranks, 2, dimensions); 
  MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 0, &comm_2d);

  // Neighbours rank id's.
	int left, right, upper, lower;
	MPI_Cart_shift(comm_2d, 0, 1, &left, &right);
	MPI_Cart_shift(comm_2d, 1, 1, &upper, &lower);

  // Problem size for a single rank.
  auto chunk_size = N / ranks_per_row;

  // Buffers for computation.
  vector<vector<double>> buffer(chunk_size, vector<double>(chunk_size, 273));
  vector<vector<double>> swap_buffer(chunk_size, vector<double>(chunk_size, 273));

  // Place heat source.
  int source = N / 4;
  int source_coords[2] = {(source / chunk_size), (source / chunk_size)};
  int source_rank;
  MPI_Cart_rank(comm_2d, source_coords, &source_rank);
  auto source_index = source % chunk_size;
  
  if (rank_id == source_rank) {
    buffer[source_index][source_index] = 273 + 60;
  }


  // Buffer to save upper/lower bordering row.
  vector<double> upper_buffer(chunk_size);
  vector<double> lower_buffer(chunk_size);

  // Iterate over timesteps.
  for (auto t = 0; t < timesteps; t++) { 

    // send to lower
    if (lower >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[chunk_size-1][0], chunk_size, MPI_DOUBLE, lower, TO_LOWER, comm_2d, &req);
      MPI_Request_free(&req);
    }
    // send to upper and recieve from upper
    if (upper >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[0][0], chunk_size, MPI_DOUBLE, upper, TO_UPPER, comm_2d, &req);
      MPI_Request_free(&req);

      MPI_Recv(&upper_buffer[0], chunk_size, MPI_DOUBLE, upper, TO_LOWER, comm_2d, MPI_STATUS_IGNORE);
    }
    // recieve from lower
    if (lower >= 0) {
      MPI_Recv(&lower_buffer[0], chunk_size, MPI_DOUBLE, lower, TO_UPPER, comm_2d, MPI_STATUS_IGNORE);
    }

    // iterate over rows
    #pragma omp parallel for
    for (auto row = 0; row < chunk_size; row++) { 

      // send first element of current row to left neighbour
      if (left >= 0) {
        MPI_Request req;
        MPI_Isend(&buffer[row][0], 1, MPI_DOUBLE, left, TO_LEFT, comm_2d, &req);
        MPI_Request_free(&req);
      }
      // send last element of current row to right neigbour
      if (right >= 0) {
        MPI_Request req;
        MPI_Isend(&buffer[row][chunk_size - 1], 1, MPI_DOUBLE, right, TO_RIGHT, comm_2d, &req);
        MPI_Request_free(&req);
      }

      // iterate over columns
      for (auto column = 0; column < chunk_size; column++) {

        if (rank_id == source_rank && (row == source_index && column == source_index)) {
            swap_buffer[row][column] = buffer[row][column];
            continue;
        }
        
        auto current_temp = buffer[row][column];
        
        auto left_temp = (column != 0) ? buffer[row][column - 1] : current_temp;
        auto right_temp = (column != chunk_size - 1) ? buffer[row][column + 1] : current_temp;
        auto upper_temp = (row != 0) ? buffer[row - 1][column] : current_temp;
        auto lower_temp = (row != chunk_size - 1) ? buffer[row + 1][column] : current_temp;

        if ((left >= 0) && (column == 0)) {
          MPI_Recv(&left_temp, 1, MPI_DOUBLE, left, TO_RIGHT, comm_2d, MPI_STATUS_IGNORE);
        }

        if ((right >= 0) && (column == chunk_size - 1)) { 
          MPI_Recv(&right_temp, 1, MPI_DOUBLE, right, TO_LEFT, comm_2d, MPI_STATUS_IGNORE);
        }

        if ((upper >= 0) && (row == 0)) {
          upper_temp = upper_buffer[column];
        }

        if ((lower >= 0) && (row == chunk_size - 1)) {
          lower_temp = lower_buffer[column];
        }

        swap_buffer[row][column] = current_temp + 0.2 * (left_temp + right_temp + upper_temp + lower_temp + (-4 * current_temp));  
      }
    }
    
    // swap matrices (just pointers, not content)
    swap(buffer, swap_buffer);
  }

  // Collect results.
  if (rank_id == 0){
    vector<vector<double>> result(N, vector<double>(N));

    // Array to save coordinates.
    int coord[2];
    
    // Add buffers from ranks to result.
    for (auto i = 1; i < amount_of_ranks; i++){
      
      auto receive_size = chunk_size * chunk_size;
      vector<double> A(receive_size, 1);
      MPI_Recv(&A[0], receive_size, MPI_DOUBLE, i, TO_MAIN, comm_2d, MPI_STATUS_IGNORE);
      
      MPI_Cart_coords(comm_2d, i, 2, coord);

      for (auto row = 0; row < chunk_size; row++) {
        for (auto column = 0; column < chunk_size; column++) {
          result[row + coord[1] * chunk_size][column + coord[0] * chunk_size] = A[column + row * chunk_size];
        }
      }
    }

    // Add buffer of rank 0 to result.
    MPI_Cart_coords(comm_2d, 0, 2, coord);

    for (auto row = 0; row < chunk_size; row++) {
      for (auto column = 0; column < chunk_size; column++) {
        result[row + coord[1] * chunk_size][column + coord[0] * chunk_size] = buffer[row][column];
      }
    }

    printTemperature(result, N, 80, 50);

    // verification
    if (verify2d(result, N)) {
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
    auto send_size = chunk_size * chunk_size;

    vector<double> to_send;
    for (auto row = 0; row < chunk_size; row++) {
      for (auto column = 0; column < chunk_size; column++) {
        to_send.push_back(buffer[row][column]);
      }
    }
    MPI_Send(&to_send[0], send_size, MPI_DOUBLE, 0, TO_MAIN, comm_2d);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}