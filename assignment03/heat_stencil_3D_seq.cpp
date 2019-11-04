#include "../shared/stencil.hpp"

#include <chrono>

using namespace std;

int main(int argc, char **argv) {

  // problem size
  auto problem_size = 50;
  if (argc > 1) {
    problem_size = strtol(argv[1], nullptr, 10);
  }
  auto timesteps = problem_size * 100;
  cout << "Computing heat-distribution with problem size " << problem_size << "^3" << " for " << timesteps << " timesteps." << endl;

  auto start_time = chrono::high_resolution_clock::now();

  // init matrix of size N*N
  vector<vector<vector<double>>> buffer_a(problem_size, vector<vector<double>>(problem_size, vector<double>(problem_size, 273)));
  vector<vector<vector<double>>> buffer_b(problem_size, vector<vector<double>>(problem_size, vector<double>(problem_size, 273)));

  auto source = problem_size / 4;
  buffer_a[source][source][source] = 273 + 60;

  for (auto t = 0; t < timesteps; t++) {
    for (auto slice = 0; slice < problem_size; slice++) {
      for (auto row = 0; row < problem_size; row++) {
        for (auto column = 0; column < problem_size; column++){

          if ((slice == source) && (row == source) && (column == source)) {
            buffer_b[slice][row][column] = buffer_a[slice][row][column];
            continue;
          }

          auto t_current = buffer_a[slice][row][column];
          
          auto t_upper = (row != 0) ? buffer_a[slice][row - 1][column] : t_current;
          auto t_lower = (row != problem_size - 1) ? buffer_a[slice][row + 1][column] : t_current;
          auto t_left = (column != 0) ? buffer_a[slice][row][column - 1] : t_current;
          auto t_right = (column != problem_size - 1) ? buffer_a[slice][row][column + 1] : t_current;
          auto t_before = (slice != 0) ? buffer_a[slice - 1][row][column] : t_current;
          auto t_behind = (slice != problem_size - 1) ? buffer_a[slice + 1][row][column] : t_current;

          auto temp = t_current + 0.14 * (t_left + t_right + t_upper + t_lower + t_before + t_behind + (-6 * t_current));

          buffer_b[slice][row][column] = temp;
        }
      }
    }

    // swap matrices (just pointers, not content)
    buffer_b.swap(buffer_a);

    if (!(t % 2000)) {
      cout << "Step t= " << t << endl;
      printTemperature(buffer_a[source], problem_size, 50, 30);
      cout << endl << endl;
    }
  }

  cout << "Final:" << endl;
  printTemperature(buffer_a[0], problem_size, 50, 30);
  printTemperature(buffer_a[source], problem_size, 50, 30);
  printTemperature(buffer_a[problem_size - 1], problem_size, 50, 30);

  // verification
  if (verify3d(buffer_a, problem_size)) {
    cout << "VERIFICATION: SUCCESS!" << endl;
  } else {
    cout << "VERIFICATION: FAILURE!" << endl;
  }

  // Measure time.
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
  cout << endl;
  cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}