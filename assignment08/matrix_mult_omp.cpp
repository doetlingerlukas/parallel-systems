#include "../shared/matrix.hpp"
#include <chrono>

using namespace std;

Matrix multiplyMatrices(Matrix A, Matrix B);
void printMatrix(Matrix M);

int main(int argc, char** argv) {

  unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
  unsigned seed2 = chrono::system_clock::now().time_since_epoch().count() / 2;
  mt19937 genA(seed1);
  mt19937 genB(seed2);

  Matrix A(2, 2);
  Matrix B(2, 2);
  A.randomInit(genA);
  B.randomInit(genB);

  // Terminate if matrices can't be multiplied.
  if (A.columns != B.rows) {
    cout << "Number of column at matrix A has to be equal to the amount of rows in matrix B!" << endl;
    return EXIT_FAILURE;
  }

  cout << "Matrix A:" << endl;
  printMatrix(A);
  cout << endl << "Matrix B:" << endl;
  printMatrix(B);
  cout << endl << "A * B:" << endl;
  Matrix result = multiplyMatrices(A, B);
  printMatrix(result);

  return EXIT_SUCCESS;
}

Matrix multiplyMatrices(Matrix A, Matrix B) {
  Matrix result(A.rows, B.columns);

  #pragma omp parallel for
  for(size_t a_row = 0; a_row < A.rows; a_row++) {
    for(size_t b_col = 0; b_col < B.columns; b_col++) {
      for(size_t a_col = 0; a_col < A.columns; a_col++) {
        result.data[a_row][b_col] += A.data[a_row][a_col] * B.data[a_col][b_col];
      }
    }
  }

  return result;
}

void printMatrix(Matrix M) {

  for(size_t row = 0; row < M.rows; row++) {
    for(size_t col = 0; col < M.columns; col++) {
      cout << M.data[row][col] << " ";
    }
    cout << endl;
  }
}