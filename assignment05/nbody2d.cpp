#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>
#include <cassert>

using namespace std;

constexpr double G = 1;
constexpr double M = 1;

double fRand(double min, double max);

class Particle {

  double vx, vy; // velocity
  double fx, fy; // force
  double m = M; // mass

  public:
    double px, py; // position

    Particle(){}

    Particle(int Nx, int Ny){
      px = fRand(0, Nx);
      py = fRand(0, Ny);
      vx = fRand(0, 1.0);
      vy = fRand(0, 1.0);
    }

    void printParticle(){
      cout << "position: (" << px << ", " << py << ")" <<endl;;
    }

    double getRadius(Particle b){
      double dx = px - b.px;
      double dy = py - b.py;
      double r = sqrt(dx*dx + dy*dy);
      return r;
    }

    void calculateForce(Particle b){
      double rad = getRadius(b);
      assert(rad > 0.0);
      double F = G * (m * b.m) / (rad * rad);
      fx = F * (px - b.px) / rad;
      fy = F * (py - b.py) / rad;
    }

    void update(int Nx, int Ny){
      vx += fx / m;
      vy += fy / m;
      px += vx;
      py += vy;
      
      // handle egde cases, flip velocity sign if on border
      if (px < 0.0){
        px = 0.0;
        vx = -vx;
      } else if (px > Nx-1){
        px = Nx-1;
        vx = -vx;
      }
      if (py < 0.0){
        py = 0.0;
        vy = -vy;
      } else if (py > Ny-1){
        py = Ny-1;
        vy = -vy;
      }
    }

    void resetForce(){
      fx = 0.0;
      fy = 0.0;
    }
};

void printParticleVector(vector<Particle> particles);
void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny);

int main(){

  //room size
  int Nx = 20;
  int Ny = 20;

  //number of particles
  int N = 10;

  int timesteps = N;

  srand(time(NULL));

  vector<Particle> particles;
  particles.reserve(N);
  for (auto i = 0; i < N; i++){
    particles.emplace_back(Nx, Ny);
  }

  for (auto t = 0; t < timesteps; t++) {
    
    for (int i = 0; i < N; ++i) {
      particles[i].resetForce();
      for (int j = 0; j < N; ++j) {
        if (i != j) {
          particles[i].calculateForce(particles[j]);
        }
      }
      particles[i].update(Nx, Ny);
    }

    cout << "timestep :" << t << endl;

    printParticleVector2D(particles, N, Nx, Ny);

    // sleep to see movement happen
    //this_thread::sleep_for(std::chrono::milliseconds(500));
  }

}

double fRand(double min, double max){
  double f = ((double)rand()-1) / RAND_MAX;
  return min + f * (max - min);
}

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