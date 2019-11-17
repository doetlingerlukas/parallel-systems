#include "./particle.hpp"

#include <vector>
#include <cmath>

using namespace std;

void printParticleVector(vector<Particle> particles){
  for (size_t i = 0; i < particles.size(); i++){
    particles[i].printParticle();
  }
}

void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny){
  const char *colors = " .-:=+*^X#%@";
  const int numColors = 12;

  vector<vector<int>> result2D(Ny, vector<int>(Nx, 0));
  const double widthStep = 1.0 / (double) Nx;
  const double heightStep = 1.0 / (double) Ny;

  // set particles in result
  for (auto i = 0; i < N; i++){
    double y = floor(particles[i].py / heightStep);
    double x = floor(particles[i].px / widthStep);
    cout << particles[i].py << " " << particles[i].px << endl;
    result2D[y > 0 ? y-1 : y][x > 0 ? x-1 : x] += 1;
  }

  // print upper border
  for (auto i = 0; i < Nx+2; i++) {
    cout << "-";
  }

  // print 2D result vector
  cout << endl;
  for (auto i = 0; i < Ny; i++) {
    cout << "|";
    for (auto j = 0; j < Nx; j++) {
      char c = result2D[i][j] >= numColors ? colors[numColors-1] : colors[result2D[i][j]];
      cout << c;
    }
    cout << "|" << endl;
  }

  // print lower border
  for (auto i = 0; i < Nx+2; i++) {
    cout << "-";
  }
  cout << endl;
}