#include <iostream>
#include <vector>

using namespace std;

// boundaries for temperature (for simplicity hard-coded)
static const double SOURCE_TEMP = 273 + 60;
static const double MAX_TEMP = 273 + 30;
static const double MIN_TEMP = 273 + 0;

void printTemperature(vector<vector<double>> m, int problem_size, int res_width, int res_height) {
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  int width_step = problem_size > res_width ? problem_size / res_width : 1;
  int height_step = problem_size > res_height ? problem_size / res_height : 1;
 
  // top wall
  for (auto wall = 0; wall < res_width + 2; wall++) {
    cout << "-";
  }
  cout << endl;
  
  for (auto row = 0; row < res_height; row++){
    // left wall
    cout << "|";
    
    for (auto column = 0; column < res_width; column++) {
      double max_t = 0;
      for (auto x = height_step * row; x < height_step * row + height_step; x++) {
        for (auto y = width_step * column; y < width_step * column + width_step; y++) {
          max_t = (max_t < m[x][y]) ? m[x][y] : max_t;
        }
      }
      double temp = max_t <= MAX_TEMP ? max_t : MAX_TEMP - 1;

      int c = ((temp - MIN_TEMP) / (MAX_TEMP - MIN_TEMP)) * numColors;
      c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);
      cout << colors[c];
    }
    // right wall
    cout << "|" << endl;
  }

  // bottom wall
  for (auto wall = 0; wall < res_width + 2; wall++) {
    cout << "-";
  }
  cout << endl;
}

bool verify2d(vector<vector<double>> m, int problem_size) {
  bool result = true;

  for (auto row = 0; row < problem_size; row++) {
    for (auto column = 0; column < problem_size; column++){
      auto temp = m[row][column];

      if (temp < MIN_TEMP || temp > SOURCE_TEMP) {
        result = false;
      }
    }
  }

  return result;
}

bool verify3d(vector<vector<vector<double>>> m, int problem_size) {
  bool result = true;

  for (auto slice = 0; slice < problem_size; slice++) {
    result = verify2d(m[slice], problem_size);
  }

  return result;
}