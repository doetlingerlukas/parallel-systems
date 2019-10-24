#include <iostream>
#include <vector>

#define RESOLUTION 80

using namespace std;

void printTemperature(vector<vector<vector<double>>> m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 25;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 2;
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

            auto t_left = (i != 0) ? A[i - 1][j][k] : t_current;
            auto t_right = (i != N - 1) ? A[i + 1][j][k] : t_current;
            auto t_upper = (j != 0) ? A[i][j - 1][k] : t_current;
            auto t_lower = (j != N - 1) ? A[i][j + 1][k] : t_current;
            auto t_before = (k != 0) ? A[i][j][k - 1] : t_current;
            auto t_behind = (k != N - 1) ? A[i][j][k + 1] : t_current;

            auto temp = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + t_before + t_behind - 6 * t_current);
            B[i][j][k] = temp;

          }
  
      }
  
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
  const double min = 0;

  // set the 'render' resolution
  int W = RESOLUTION;
  if (N < RESOLUTION){
    W = N;
  }

  // step size in each dimension
  int sW = N / W;

  cout << "Final: " << endl;
 
  cout << "|";
  
  for (auto i = 0; i < N; i++){
    for (auto j = 0; j < N; j++) {
        for (auto k = 0; k < W; k++){
            double max_t = 0;
            for (auto x = sW * k; x < sW * k + sW; x++) {
                max_t = (max_t < m[i][j][k]) ? m[i][j][k] : max_t;
            }
            double temp = max_t;

            // pick the 'color'
            int c = ((temp - min) / (max - min)) * numColors;
            c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

            cout << colors[c];
        }
        cout << endl;
    }
    cout << endl;
  }

  cout << "|";
}
