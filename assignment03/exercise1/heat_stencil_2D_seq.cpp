#include <iostream>
#include <vector>

#define RESOLUTION 80

using namespace std;

void printTemperature(vector<vector<double>> m, int N);

int main(int argc, char **argv) {

  // problem size
  auto N = 100;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 100;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << " for T=" << T << " timesteps." << endl;

  // init matrix of size N*N
  vector<vector<double>> A(N);
  vector<vector<double>> B(N);
  for (auto i = 0; i < N; i++){
		A[i].resize(N, 273);
    B[i].resize(N, 273);
  }

  auto source_x = N/4;
  auto source_y = N/4;
  A[source_x][source_y] = 273 + 60;

  for (auto t = 0; t < T; t++) { // iterate timesteps
    for (auto i = 0; i < N; i++) { // iterate rows
      for (auto j = 0; j < N; j++) { // iterate columns

        if (i == source_x && j == source_y) {
            B[i][j] = A[i][j];
            continue;
        }

        auto t_current = A[i][j];

        auto t_left = (i != 0) ? A[i - 1][j] : t_current;
        auto t_right = (i != N - 1) ? A[i + 1][j] : t_current;
        auto t_upper = (j != 0) ? A[i][j - 1] : t_current;
        auto t_lower = (j != N) ? A[i][j + 1] : t_current;

        B[i][j] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower - 4 * t_current);
      }
    }
    if (!(t % 1000)) {
      cout << "Step t= " << t << endl;
      printTemperature(A, N);
      cout << endl << endl;
    }

    // swap matrices (just pointers, not content)
    swap(A, B);
  }

  return EXIT_SUCCESS;
}


void printTemperature(vector<vector<double>> m, int N) {
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
 
  cout << "|";
  
  for (auto i = 0; i < N; i++){
    for (auto j = 0; j < W; j++) {
      
      double max_t = 0;
      for (auto x = sW * j; x < sW * j + sW; x++) {
        max_t = (max_t < m[i][j]) ? m[i][j] : max_t;
      }
      double temp = max_t;

      // pick the 'color'
      int c = ((temp - min) / (max - min)) * numColors;
      c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

      cout << colors[c];
    }
    cout << endl;
  }

  cout << "|";
}
