#include "./particle.hpp"

#include <iostream>
#include <vector>

using namespace std;

void printParticleVector(vector<Particle> particles){
  for (size_t i = 0; i < particles.size(); i++){
    particles[i].printParticle();
  }
}

void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny){
  vector<vector<char>> result2D(Nx, vector<char>(Ny, ' '));
  for (auto i = 0; i < N; i++){
    Particle temp = particles[i];
    //cout << temp.py << ", " << temp.py<<endl;
    result2D[temp.px][temp.py] = 'x';
  }

  for (auto i = 0; i <= Nx+1; i++){
    cout << "-";
  }
  cout << endl;

  for (auto i = 0; i < Nx; i++){
    cout << "|";
    for (auto j = 0; j < Ny; j++){
      cout << result2D[i][j];
    }
    cout << "|" << endl;
  }

  for (auto i = 0; i <= Nx+1; i++){
    cout << "-";
  }
  cout << endl;
}