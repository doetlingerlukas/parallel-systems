#include <iostream>
#include <vector>
#include <random>

using namespace std;

class Matrix {

  public:
    size_t rows, columns;
    vector<vector<long>> data;

    Matrix(size_t r, size_t c) {
      rows = r;
      columns = c;
      data = vector<vector<long>>(rows, vector<long>(columns, 0));
    }

    void randomInit(mt19937 gen) {
      uniform_int_distribution<int> dist(1, 10);

      for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
          data[i][j] = dist(gen);
        }
      }
    }

    void print() {
      for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < columns; col++) {
          cout << data[row][col] << " ";
        }
        cout << endl;
      }
    }

    Matrix multiplyWith(Matrix B) {
      Matrix result(rows, B.columns);

      #pragma omp parallel for
      for(size_t a_row = 0; a_row < rows; a_row++) {
        for(size_t b_col = 0; b_col < B.columns; b_col++) {
          for(size_t a_col = 0; a_col < columns; a_col++) {
            result.data[a_row][b_col] += data[a_row][a_col] * B.data[a_col][b_col];
          }
        }
      }
      return result;
    }
};