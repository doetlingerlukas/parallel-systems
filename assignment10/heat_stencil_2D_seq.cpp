#include "../shared/stencil.hpp"

#include <chrono>

using namespace std;

int main(int argc, char **argv) {

  // problem size
  auto N = 100;
  bool print = false;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
    if (argc > 2) {
      string verbose = argv[2];
      print = verbose == "--verbose";
    }
  }
  auto T = N * 100;
  cout << "Computing heat-distribution for room size N=" << N << "*"<< N << " for T=" << T << " timesteps." << endl;

  auto start_time = chrono::high_resolution_clock::now();

  // init matrix of size N*N
  vector<vector<double>> A(N, vector<double>(N, 273));
  vector<vector<double>> B(N, vector<double>(N, 273));

  auto source_x = N/4;
  auto source_y = N/4;
  A[source_x][source_y] = 273 + 60;

  for (auto t = 0; t < T; t++) {
    for (auto i = 0; i < N; i++) {
      for (auto j = 0; j < N; j++) {

        if (i == source_x && j == source_y) {
            B[i][j] = A[i][j];
            continue;
        }

        auto t_current = A[i][j];

        auto t_upper = (i != 0) ? A[i - 1][j] : t_current;
        auto t_lower = (i != N - 1) ? A[i + 1][j] : t_current;
        auto t_left = (j != 0) ? A[i][j - 1] : t_current;
        auto t_right = (j != N - 1) ? A[i][j + 1] : t_current;

        B[i][j] = t_current + 0.2 * (t_left + t_right + t_upper + t_lower + (-4 * t_current));
      }
    }
    if ((!(t % 1000) && print) || t == T-1) {
      cout << "Step t= " << t << endl;
      printTemperature(A, N, 80, 50);
      cout << endl << endl;
    }

    // swap matrices (just pointers, not content)
    swap(A, B);
  }

  // verification
  if (verify2d(A, N)) {
    cout << "VERIFICATION: SUCCESS!" << endl;
  } else {
    cout << "VERIFICATION: FAILURE!" << endl;
  }

  // Measure time.
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
    cout << endl;
    cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}