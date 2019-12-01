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
      uniform_int_distribution<int> dist(1, 100);

      for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
          data[i][j] = dist(gen);
        }
      }
    }
};