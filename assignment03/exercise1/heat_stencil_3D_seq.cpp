#include <iostream>
#include <vector>

#define RESOLUTION 80

using namespace std;

void printTemperature(vector<vector<vector<double>>> m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 20;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 5;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << "*" << N << " for T=" << T << " timesteps." << endl;

  // init matrix of size N*N
  vector<vector<vector<double>>> A(N);
  vector<vector<vector<double>>> B(N);
  for (auto i = 0; i < N; i++){
      A[i].resize(N);
      B[i].resize(N);
      for (auto j = 0; j < N; j++){
		    A[i][j].resize(N, 273);
        B[i][j].resize(N, 273);
      }
  }

  auto source_x = N/4;
  auto source_y = N/4;
  auto source_z = N/4;
  A[source_x][source_y][source_z] = 273 + 60;

  for (auto t = 0; t < T; t++) { // iterate timesteps
    for (auto i = 0; i < N; i++) {
      for (auto j = 0; j < N; j++) {
          for (auto k = 0; k < N; k++){
            if (i == source_x && j == source_y && k == source_z) {
                B[i][j][k] = A[i][j][k];
                continue;
            }

            auto t_current = A[i][j][k];

            auto t_upper = (i != 0) ? A[i - 1][j][k] : t_current;
            auto t_lower = (i != N - 1) ? A[i + 1][j][k] : t_current;
            auto t_left = (j != 0) ? A[i][j - 1][k] : t_current;
            auto t_right = (j != N - 1) ? A[i][j + 1][k] : t_current;
            auto t_before = (k != 0) ? A[i][j][k - 1] : t_current;
            auto t_behind = (k != N - 1) ? A[i][j][k + 1] : t_current;

            auto temp = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + t_before + t_behind - 6 * t_current);

            B[i][j][k] = temp;
          }
      }
    }
    if (!(t % 20)) {
      cout << "Step t= " << t << endl;
      printTemperature(A, N);
      cout << endl << endl;
    }

    // swap matrices (just pointers, not content)
    swap(A, B);
  }

  printTemperature(A, N);

  return EXIT_SUCCESS;
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
