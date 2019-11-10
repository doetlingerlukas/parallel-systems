// particle.hpp
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Particle {

  double vx, vy; // velocity
  double fx, fy; // force
  // mass, defaults to 1
  double m = 1;

  public:
    double px, py; // position

    Particle(){}
    Particle(int Nx, int Ny);
    Particle(int Nx, int Ny, int M);

    void printParticle();
    double getRadius(Particle b);
    void calculateForce(Particle b);
    void update(int Nx, int Ny);
    void resetForce();
};

#endif