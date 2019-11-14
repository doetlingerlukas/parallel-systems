#include "../shared/particle.hpp"

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;


double fRand(double min, double max){
  double f = ((double)rand()-1) / RAND_MAX;
  return min + f * (max - min);
}

Particle::Particle(int Nx, int Ny) {
  px = fRand(0, Nx);
  py = fRand(0, Ny);
  vx = fRand(0, 1.0);
  vy = fRand(0, 1.0);
}

Particle::Particle(int Nx, int Ny, int M) {
  px = fRand(0, Nx);
  py = fRand(0, Ny);
  vx = fRand(0, 1.0);
  vy = fRand(0, 1.0);
  m = M;
}

void Particle::printParticle() {
  cout << "position: (" << px << ", " << py << ")" <<endl;
}

double Particle::getRadius(Particle b){
  double dx = px - b.px;
  double dy = py - b.py;
  double r = sqrt(dx*dx + dy*dy);
  return r;
}

void Particle::calculateForce(Particle b){
  double rad = getRadius(b);
  assert(rad > 0.0);
  double F = G * (m * b.m) / (rad * rad);
  fx = F * (px - b.px) / rad;
  fy = F * (py - b.py) / rad;
}

void Particle::update(int Nx, int Ny){
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
