#include <iostream>
#include <vector>
#include <random>

using namespace std;

typedef struct {
  size_t rows;
  size_t columns;
} matrix_size;

vector<vector<long>> multiplyMatrices(vector<vector<long>> A, vector<vector<long>> B);
void printMatrix(vector<vector<long>> m, matrix_size size);

int main(int argc, char** argv) {

  matrix_size default_size = { 10, 10 };

  matrix_size A_size = default_size;
  matrix_size B_size = default_size;

  // Terminate if matrices can't be multiplied.
  if (A_size.columns != B_size.rows) {
    cout << "Number of column at matrix A has to be equal to the amount of rows in matrix B!" << endl;
    return EXIT_FAILURE;
  }

  // Initialize matrices randomly.
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dist(1, 100);

  vector<vector<long>> A(A_size.rows, vector<long>(A_size.columns, 0));
  vector<vector<long>> B(B_size.rows, vector<long>(B_size.columns, 0));

  for(size_t i = 0; i < A_size.rows; i++) {
    for(size_t j = 0; j < A_size.columns; i++) {
      A[i][j] = dist(gen);
    }
  }
  for(size_t i = 0; i < B_size.rows; i++) {
    for(size_t j = 0; j < B_size.columns; i++) {
      B[i][j] = dist(gen);
    }
  }

  vector<vector<long>> result = multiplyMatrices(A, B);


  return EXIT_SUCCESS;
}

vector<vector<long>> multiplyMatrices(vector<vector<long>> A, vector<vector<long>> B) {
  vector<vector<long>> result(A.size(), vector<long>(B[0].size(), 0));

  #pragma omp parallel for
  for(size_t a_row = 0; a_row < A.size(); a_row++) {
    for(size_t b_col = 0; b_col < B[0].size(); b_col++) {
      for(size_t a_col = 0; a_col < A[0].size(); a_col++) {
        result[a_row][b_col] += A[a_row][a_col] * B[a_col][b_col];
      }
    }
  }

  return result;
}

void printMatrix(vector<vector<long>> m, matrix_size size) {

  // Print upper border.
  for(size_t i = 0; i < m[0].size() + 2; i++) {

  }
}