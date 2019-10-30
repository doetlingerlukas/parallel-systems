#include <iostream>
#include <vector>
#include <mpi.h>

#define RESOLUTION 120

using namespace std;

void printTemperature(vector<double> m, int N);

int main(int argc, char **argv) {

  MPI_Init(0,0);
  double start = MPI_Wtime();

  // problem size
  auto N = 5000;
  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }
  auto T = N * 500;
  cout << "Computing heat-distribution for room size N=" << N << " for T=" << T << " timesteps." << endl;

  // initial temp fileds
  vector<double> A (N);

  for (auto i = 0; i < N; i++) {
    A[i] = 273; // temperature is 0° C everywhere (273 K)
  }

  // heat source
  int source_x = N / 4;
  A[source_x] = 273 + 60;

  cout << "Initial:\t"; 
  printTemperature(A, N);
  cout << endl;

  // ---------- compute ----------
  // buffer for the computation
  vector<double> B (N);

  for (auto t = 0; t < T; t++) {
    for (auto i = 0; i < N; i++) {
      
      if (i == source_x) {
        B[i] = A[i];
        continue;
      }
      auto t_current = A[i];

      auto t_left = (i != 0) ? A[i - 1] : t_current;
      auto t_right = (i != N - 1) ? A[i + 1] : t_current;

      B[i] = t_current + 0.2 * (t_left + t_right + (-2 * t_current));
    }

    // swap matrices (just pointers, not content)
    vector<double> H = A;
    A = B;
    B = H;

    // show intermediate step
    /*
    if (!(t % 1000)) {
      cout << "Step t= " << t << "\t";
      printTemperature(A, N);
      cout << endl;
    }
    */
  }

  // ---------- check ----------

  cout << "Final:\t\t";
  printTemperature(A, N);
  cout << endl;

  auto success = 1;
  for (auto i = 0; i < N; i++) {
    auto temp = A[i];
    if (273 <= temp && temp <= 273 + 60)
      continue;
    success = 0;
    break;
  }

  cout << "Verification: " << ((success) ? "OK" : "FAILED") << endl;

  cout << "elapsed time: " << MPI_Wtime() - start << endl;
  MPI_Finalize();
  return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void printTemperature(vector<double> m, int N) {
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  // boundaries for temperature (for simplicity hard-coded)
  const double max = 273 + 30;
  const double min = 273 + 0;

  // set the 'render' resolution
  int W = RESOLUTION;

  // step size in each dimension
  int sW = N / W;

 
  cout << "|";
  
  for (auto i = 0; i < W; i++) {
    
    double max_t = 0;
    for (int x = sW * i; x < sW * i + sW; x++) {
      max_t = (max_t < m[x]) ? m[x] : max_t;
    }
    double temp = max_t;

    // pick the 'color'
    int c = ((temp - min) / (max - min)) * numColors;
    c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

    cout << colors[c];
  }

  cout << "|";
}
