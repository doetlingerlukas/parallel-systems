#include "./particle.hpp"

#include <vector>
#include <cmath>

using namespace std;

void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny){
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;
  const double resX = 50.0;
  const double resY = 20.0;

  vector<vector<int>> result2D(resY, vector<int>(resX, 0));
  const double widthStep = (double) Nx / resX;
  const double heightStep = (double) Ny / resY;

  // set particles in result
  for (auto i = 0; i < N; i++){
    double y = floor(particles[i].py / heightStep);
    double x = floor(particles[i].px / widthStep);
    result2D[y > 0 ? y-1 : y][x > 0 ? x-1 : x] += 1;
  }

  // print upper border
  for (auto i = 0; i < resX+2; i++) {
    cout << "-";
  }

  // print 2D result vector
  cout << endl;
  for (auto i = 0; i < resY; i++) {
    cout << "|";
    for (auto j = 0; j < resX; j++) {
      char c = result2D[i][j] >= numColors ? colors[numColors-1] : colors[result2D[i][j]];
      cout << c;
    }
    cout << "|" << endl;
  }

  // print lower border
  for (auto i = 0; i < resX+2; i++) {
    cout << "-";
  }
  cout << endl;
}