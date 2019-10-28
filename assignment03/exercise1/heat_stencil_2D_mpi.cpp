#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <stdio.h> 

#define RESOLUTION 80

const static int TO_LEFT = 0;
const static int TO_RIGHT = 1;
const static int TO_UPPER = 2;
const static int TO_LOWER = 3;
const static int TO_MAIN = 4;

using namespace std;

void printTemperature(vector<vector<double>> m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 100; // has to be devisable by 4
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 100;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << " for T=" << T << " timesteps." << endl;

  int rank_id, number_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_ranks);

  if (fmod(sqrtf(number_ranks), 1) != 0) {
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  int ranks_per_row = sqrt(number_ranks);

  // set up grid
  int dims[2] = {ranks_per_row, ranks_per_row};
	int periods[2] = {0, 0}; // to avoid too many edge cases in code
  //int mycoords[2];
  MPI_Comm comm_2d;
  MPI_Dims_create(number_ranks, 2, dims); 
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_2d);

  // define neighbors
	int left, right, upper, lower;
	MPI_Cart_shift(comm_2d, 0, 1, &left, &right);
	MPI_Cart_shift(comm_2d, 1, 1, &upper, &lower);

  cout << "rank: " << rank_id << "| left: " << left << "| right: " << right << "| upper: " << upper << "| lower: " << lower << endl;

  // split problem size among ranks
  auto N_rank = N / ranks_per_row;

  // init rank buffer
  vector<vector<double>> buffer(N_rank);
  vector<vector<double>> swap_buffer(N_rank);
  for (auto i = 0; i < N_rank; i++){
		buffer[i].resize(N_rank, 273);
    swap_buffer[i].resize(N_rank, 273);
  }

  // source
  int source = N / 4;
  int source_coords[2] = {(source / N_rank), (source / N_rank)};
  int source_rank;
  MPI_Cart_rank(comm_2d, source_coords, &source_rank);
  auto source_index = source % N_rank;

  if (rank_id == source_rank) {
    buffer[source_index][source_index] = 273 + 60;
    cout << rank_id << " " << source_index << endl;
  }


  // ---------- compute -------------
  vector<double> upper_buffer(N_rank);
  vector<double> lower_buffer(N_rank);

  for (auto t = 0; t < T; t++) { 

    // send to lower
    if (lower >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[N_rank-1][0], N_rank, MPI_DOUBLE, lower, TO_LOWER, comm_2d, &req);
      MPI_Request_free(&req);
    }
    // send to upper and recieve from upper
    if (upper >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[0][0], N_rank, MPI_DOUBLE, upper, TO_UPPER, comm_2d, &req);
      MPI_Request_free(&req);

      MPI_Recv(&upper_buffer[0], N_rank, MPI_DOUBLE, upper, TO_LOWER, comm_2d, MPI_STATUS_IGNORE);
    }

    // iterate over rows
    for (auto i = 0; i < N_rank; i++) { 

      // send first element of current row to left neighbour
      if (left >= 0) {
        MPI_Request req;
        MPI_Isend(&buffer[i][0], 1, MPI_DOUBLE, left, TO_LEFT, comm_2d, &req);
        MPI_Request_free(&req);
      }
      // send last element of current row to right neigbour
      if (right >= 0) {
        MPI_Request req;
        MPI_Isend(&buffer[i][N_rank - 1], 1, MPI_DOUBLE, right, TO_RIGHT, comm_2d, &req);
        MPI_Request_free(&req);
      }

      // recieve from lower
      if ((lower >= 0) && (i == N_rank - 1)) {
        MPI_Recv(&lower_buffer[0], N_rank, MPI_DOUBLE, lower, TO_UPPER, comm_2d, MPI_STATUS_IGNORE);
      }

      // iterate over columns
      for (auto j = 0; j < N_rank; j++) {

        if (rank_id == source_rank && (i == source_index && j == source_index)) {
            swap_buffer[i][j] = buffer[i][j];
            continue;
        }
        
        auto t_current = buffer[i][j];
        
        auto t_left = (j != 0) ? buffer[i][j - 1] : t_current;
        auto t_right = (j != N_rank - 1) ? buffer[i][j + 1] : t_current;

        // recieve from left
        if ((left >= 0) && (j == 0)) {
          MPI_Recv(&t_left, 1, MPI_DOUBLE, left, TO_RIGHT, comm_2d, MPI_STATUS_IGNORE);
        }
        // recieve from right
        if ((right >= 0) && (j == N_rank - 1)) { 
          MPI_Recv(&t_right, 1, MPI_DOUBLE, right, TO_LEFT, comm_2d, MPI_STATUS_IGNORE);
        }

        auto t_upper = (i != 0) ? buffer[i - 1][j] : t_current;
        if ((upper >= 0) && (j == 0)) {
          t_upper = upper_buffer[i];
        }

        auto t_lower = (i != N_rank - 1) ? buffer[i + 1][j] : t_current;
        if ((lower >= 0) && (j == N_rank - 1)) {
          t_lower = lower_buffer[i];
        }

        swap_buffer[i][j] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + (-4 * t_current));  
      }
    }
    
    // swap matrices (just pointers, not content)
    swap(buffer, swap_buffer);
  }
  /*
  cout << "-----------------------------" << endl;
  cout << "rank id " << rank_id << endl;
  printTemperature(buffer, N_rank);
  cout << "-----------------------------" << endl;
  */
  
  MPI_Datatype rank_subarray;
  int size[2]    = {N, N};
  int subsize[2] = {N_rank, N_rank}; 
  int start[2] = {0, 0}; 
  MPI_Type_create_subarray(2, size, subsize, start, MPI_ORDER_C, MPI_DOUBLE, &rank_subarray);
  MPI_Type_commit(&rank_subarray);

  if (rank_id == 0){ // collect rank_buffers from all nodes
    vector<vector<double>> result(N);
    for (auto i = 0; i < N; i++){
		  result[i].resize(N);
    }
    //vector<double> flat_result(N*N);
    for (auto i = 1; i < number_ranks; i++){
      
      //MPI_Recv(&flat_result[0] + N_rank * i, (N_rank * N_rank), MPI_DOUBLE, i, TO_MAIN, comm_2d, MPI_STATUS_IGNORE);
      auto recieve_size = N_rank * N_rank;
      vector<double> A(recieve_size, 1);
      MPI_Recv(&A[0], recieve_size, MPI_DOUBLE, i, TO_MAIN, comm_2d, MPI_STATUS_IGNORE);
      cout << A[0] << endl;
      
      // TODO merge arrays of size N_rank*N_rank in one array of size N*N
      int coord[2];
      MPI_Cart_coords(comm_2d, i, 2, coord);

      int x_start = coord[0];
      int y_start = coord[1];

      for (auto i = 0; i < N_rank; i++) { // iterate rows
        for (auto j = 0; j < N_rank; j++) { // iterate columns
          result[i + y_start * N_rank][j + x_start * N_rank] = A[j + i * N_rank];
        }
      }
    }
    //add rank 0 results
    int coord[2];
    MPI_Cart_coords(comm_2d, 0, 2, coord);

    int x_start = coord[0];
    int y_start = coord[1];

    for (auto i = 0; i < N_rank; i++) { // iterate rows
      for (auto j = 0; j < N_rank; j++) { // iterate columns
        result[i + y_start * N_rank][j + x_start * N_rank] = buffer[i][j];
      }
    }

    printTemperature(result, N);
  } else { 
    // send rank_buffer to rank 0
    auto send_size = N_rank * N_rank;

    vector<double> to_send;
    for (auto i = 0; i < N_rank; i++) {
      for (auto j = 0; j < N_rank; j++) {
        to_send.push_back(buffer[i][j]);
      }
    }
    
    MPI_Send(&to_send[0], send_size, MPI_DOUBLE, 0, TO_MAIN, comm_2d);
    cout << "rank " << rank_id << " sended subarray" << endl;
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
