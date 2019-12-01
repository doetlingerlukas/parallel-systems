#include "../shared/matrix.hpp"
#include <chrono>

using namespace std;

int main(int argc, char** argv) {

  auto start_time = chrono::high_resolution_clock::now();
  unsigned int N = 10;
  bool print = false;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
    if (argc > 2) {
      string verbose = argv[2];
      print = verbose == "--verbose";
    }
  }

  unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
  unsigned seed2 = chrono::system_clock::now().time_since_epoch().count() / 2;
  mt19937 genA(seed1);
  mt19937 genB(seed2);

  Matrix A(N, N);
  Matrix B(N, N);
  A.randomInit(genA);
  B.randomInit(genB);

  // Terminate if matrices can't be multiplied.
  if (A.columns != B.rows) {
    cout << "Number of column at matrix A has to be equal to the amount of rows in matrix B!" << endl;
    return EXIT_FAILURE;
  }

  if (print) {
    cout << "Matrix A:" << endl;
    A.print();
    cout << endl << "Matrix B:" << endl;
    B.print();
    cout << endl << "A * B:" << endl;
    A.multiplyWith(B).print();
  } else {
    A.multiplyWith(B);
  }

  // Measure elapsed time.
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration<double>(end_time - start_time).count();
  cout << endl;
  cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}