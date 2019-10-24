#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

  // problem size
  auto N = 50;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 50;
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

            B[i][j][k] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + t_before + t_behind - 6 * t_current);
          }
      }
    }

    // swap matrices (just pointers, not content)
    swap(A, B);
  }

  return EXIT_SUCCESS;
}
