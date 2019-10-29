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
const static int TO_BEFORE = 5;
const static int TO_BEHIND = 6;

using namespace std;

void printTemperature(vector<vector<vector<double>>> m, int N);

vector<vector<double>> convert1dTo2d(vector<double> m, int N);

vector<double> convert2dTo1d(vector<vector<double>> m, int N);

vector<double> getColFrom3dAtk(vector<vector<vector<double>>> m, int k, int N, int colnr);

vector<double> getRowFrom3dAtj(vector<vector<vector<double>>> m, int j, int N, int rownr);

int main(int argc, char **argv) {

  // problem size
  auto N = 16; // has to be divisable by 8
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 5;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << " for T=" << T << " timesteps." << endl;

  int rank_id, number_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_ranks);

  if (number_ranks < 8) {
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  int cube_size_per_rank = cbrt(number_ranks);

  // set up grid
  int dims[3] = {cube_size_per_rank, cube_size_per_rank, cube_size_per_rank};
	int periods[3] = {0, 0, 0};
  MPI_Comm comm_3d;
  MPI_Dims_create(number_ranks, 3, dims); 
  MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &comm_3d);

  // define neighbors
	int left, right, upper, lower, before, behind;
	MPI_Cart_shift(comm_3d, 0, 1, &left, &right);
	MPI_Cart_shift(comm_3d, 1, 1, &upper, &lower);
  MPI_Cart_shift(comm_3d, 2, 1, &before, &behind);

  cout << "rank: " << rank_id << "| left: " << left << "| right: " << right << "| upper: " << upper << "| lower: " << lower << "| before: " << before << "| behind: " << behind << endl;

  // split problem size among ranks
  auto N_rank = N / cube_size_per_rank;

  // init rank buffer
  vector<vector<vector<double>>> buffer(N_rank);
  vector<vector<vector<double>>> swap_buffer(N_rank);
  for (auto i = 0; i < N_rank; i++){
      buffer[i].resize(N_rank);
      swap_buffer[i].resize(N_rank);
      for (auto j = 0; j < N_rank; j++){
		    buffer[i][j].resize(N_rank, 273);
        swap_buffer[i][j].resize(N_rank, 273);
      }
  }

  // source
  int source = N / 4;
  int source_coords[3] = {(source / N_rank), (source / N_rank),  (source / N_rank)};
  int source_rank;
  MPI_Cart_rank(comm_3d, source_coords, &source_rank);
  auto source_index = source % N_rank;

  if (rank_id == source_rank) {
    buffer[source_index][source_index][source_index] = 273 + 60;
    cout << rank_id << " " << source_index << endl;
  }


  // ---------- compute -------------
  vector<vector<double>> upper_buffer(N_rank);
  vector<vector<double>> lower_buffer(N_rank);
  for(auto i=0; i < N_rank; i++){
    upper_buffer[i].resize(N_rank, 273);
    lower_buffer[i].resize(N_rank, 273);
  }

  for (auto t = 0; t < T; t++) { // iterate timesteps

    // send to lower
    if (lower >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[0][0][0], N_rank*N_rank, MPI_DOUBLE, lower, TO_LOWER, comm_3d, &req);
      MPI_Request_free(&req);
    }
    // send to upper and recieve from upper
    if (upper >= 0) {
      MPI_Request req;
      MPI_Isend(&buffer[0][0][N_rank-1], N_rank*N_rank, MPI_DOUBLE, upper, TO_UPPER, comm_3d, &req);
      MPI_Request_free(&req);

      vector<double> temp(N_rank*N_rank, 273);
      MPI_Recv(&temp[0], N_rank*N_rank, MPI_DOUBLE, lower, TO_UPPER, comm_3d, MPI_STATUS_IGNORE);
      lower_buffer = convert1dTo2d(temp, N_rank);
    }

    if (lower >= 0) {
      vector<double> temp(N_rank*N_rank, 273);
      MPI_Recv(&temp[0], N_rank*N_rank, MPI_DOUBLE, upper, TO_LOWER, comm_3d, MPI_STATUS_IGNORE);
      upper_buffer = convert1dTo2d(temp, N_rank);
    }
    

    for (auto k = 0; k < N_rank; k++) {
      
      // send left
      if (left >= 0) {
        MPI_Request req;
        vector<double> col = getColFrom3dAtk(buffer, k, N_rank, N_rank-1);
        MPI_Isend(&col[0], N_rank, MPI_DOUBLE, left, TO_LEFT, comm_3d, &req);
        MPI_Request_free(&req);
      }
      // send right
      if (right >= 0) {
        MPI_Request req;
        vector<double> col = getColFrom3dAtk(buffer, k, N_rank, 0);
        MPI_Isend(&col[0], N_rank, MPI_DOUBLE, right, TO_RIGHT, comm_3d, &req);
        MPI_Request_free(&req);
      }

      vector<double> left_buffer(N_rank);
      vector<double> right_buffer(N_rank);

      // recieve from left
      if (left >= 0) {
        MPI_Recv(&left_buffer[0], N_rank, MPI_DOUBLE, left, TO_RIGHT, comm_3d, MPI_STATUS_IGNORE);
      }
      // recieve from right
      if (right >= 0) { 
        MPI_Recv(&right_buffer[0], N_rank, MPI_DOUBLE, right, TO_LEFT, comm_3d, MPI_STATUS_IGNORE);
      }

      for (auto j = 0; j < N_rank; j++) {
        
        // send behind
        if (behind >= 0) {
          MPI_Request req;
          vector<double> tmp = getRowFrom3dAtj(buffer, 0, N_rank, k);
          MPI_Isend(&tmp[0], N_rank, MPI_DOUBLE, behind, TO_BEHIND, comm_3d, &req);
          MPI_Request_free(&req);
        }
        // send before
        if (before >= 0) {
          MPI_Request req;
          vector<double> tmp = getRowFrom3dAtj(buffer, N_rank-1, N_rank, k);
          MPI_Isend(&tmp[0], N_rank, MPI_DOUBLE, before, TO_BEFORE, comm_3d, &req);
          MPI_Request_free(&req);
        }

        vector<double> behind_buffer(N_rank);
        vector<double> before_buffer(N_rank);

        // recieve from behind
        if ((behind >= 0) && (j == 0)) {
          MPI_Recv(&behind_buffer[0], N_rank, MPI_DOUBLE, behind, TO_BEFORE, comm_3d, MPI_STATUS_IGNORE);
        }
        // recieve from before
        if ((before >= 0) && (j == N_rank - 1)) { 
          MPI_Recv(&before_buffer[0], N_rank, MPI_DOUBLE, before, TO_BEHIND, comm_3d, MPI_STATUS_IGNORE);
        }

        for (auto i = 0; i < N_rank; i++){

          if (rank_id == source_rank && (i == source_index && j == source_index && source_index == k)) {
            swap_buffer[i][j][k] = buffer[i][j][k];
            continue;
          }

          auto t_current = buffer[i][j][k];

          auto t_upper = (k != N_rank-1) ? buffer[i][j][k+1] : t_current;
          if ((upper >= 0) && (k == N_rank-1)) {
            t_upper = upper_buffer[i][j];
          }

          auto t_lower = (k != 0) ? buffer[i][j][k-1] : t_current;
          if ((lower >= 0) && (k == N_rank - 1)) {
            t_lower = lower_buffer[i][j];
          }

          auto t_left = (i != N_rank-1) ? buffer[i+1][j][k] : t_current;
          if ((left >= 0) && (i == N_rank-1)) {
            t_left = left_buffer[i];
          }

          auto t_right = (i != 0) ? buffer[i-1][j][k] : t_current;
          if ((right >= 0) && (i == 0)) {
            t_right = right_buffer[i];
          }

          auto t_before = (j != 0) ? buffer[i][j-1][k] : t_current;
          if ((upper >= 0) && (i == 0)) {
            t_before = before_buffer[j];
          }

          auto t_behind = (j != N_rank - 1) ? buffer[i][j+1][k] : t_current;
          if ((behind >= 0) && (j == N_rank - 1)) {
            t_behind = behind_buffer[j];
          }

          swap_buffer[i][j][k] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + t_before + t_behind - 6 * t_current);
        }
      }
    }
    // swap matrices (just pointers, not content)
    swap(buffer, swap_buffer);
  }

  if(rank_id==0){
    cout << "-----------------------------" << endl;
    cout << "rank id " << rank_id << endl;
    printTemperature(buffer, N_rank);
    cout << "-----------------------------" << endl;
  }
  

  MPI_Finalize();
  return EXIT_SUCCESS;
}

vector<double> convert2dTo1d(vector<vector<double>> m, int N){
  vector<double> result(N);
  for (auto i = 0; i < N; i++) {
    for (auto j = 0; j < N; j++) {
      result.push_back(m[i][j]);
    }
  }

  return result;
}

vector<vector<double>> convert1dTo2d(vector<double> m, int N){
  vector<vector<double>> result(N);
  for (auto i = 0; i < N; i++) { // iterate rows
    result[i].resize(N);
    for (auto j = 0; j < N; j++) { // iterate columns
      result[i][j] = m[j + i * N];
    }
  }
  return result;
}

vector<double> getColFrom3dAtk(vector<vector<vector<double>>> m, int k, int N, int colnr){
  vector<double> result(N);
  for (auto i = 0; i < N; i++) { // iterate rows
    for (auto j = 0; j < N; j++) { // iterate columns
      if (j == colnr){
        result.push_back(m[i][j][k]);
      }
    }
  }
  return result;
}

vector<double> getRowFrom3dAtj(vector<vector<vector<double>>> m, int j, int N, int rownr){
  vector<double> tmp(N);
  for (auto i = 0; i < N; i++){
    tmp.push_back(m[i][j][rownr]); 
  }
  return tmp;
}


void printTemperature(vector<vector<vector<double>>> m, int N) {
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  // boundaries for temperature (for simplicity hard-coded)
  const double max = 273 + 30;
  const double min = 273 + 0;

  // set the 'render' resolution
  int W = RESOLUTION;
  int H = RESOLUTION;
  int D = RESOLUTION;
  if (N < RESOLUTION){
    W = N;
    H = N;
    D = N;
  }


  // step size in each dimension
  int sW = N / W;
  int sH = N / H;
  int sD = N / D;

  auto fixedDepth = (D / sD) / 2;
 
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
          max_t = (max_t < m[x][y][fixedDepth]) ? m[x][y][fixedDepth] : max_t;
        }
      }
      double temp = max_t <= max ? max_t : max - 1;

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
