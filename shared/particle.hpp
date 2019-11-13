// particle.hpp
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

constexpr double G = 1.0;
constexpr double M = 1.0;

class Particle {

  double vx, vy; // velocity
  double fx, fy; // force
  double m = M; // mass

  public:
    double px, py; // position

    Particle(){}
    Particle(int Nx, int Ny);
    Particle(int Nx, int Ny, int M);

    void printParticle();
    double getRadius(Particle b);
    void calculateForce(Particle b);
    void update(int Nx, int Ny);
};

#endif