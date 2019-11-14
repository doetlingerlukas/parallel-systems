// particle.hpp
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

constexpr double G = 1.0;
constexpr double M = 1.0;
constexpr double THRESHOLD = 5.0;

class Particle {

  double vx, vy; // velocity
  double m = M; // mass

  public:
    double px, py; // position
    double fx, fy; // force

    Particle(){}
    Particle(int Nx, int Ny);
    Particle(int Nx, int Ny, int M);

    void printParticle();
    double getRadius(Particle b);
    void calculateForce(Particle b);
    void updateForce(Particle b);
    void update(int Nx, int Ny);
    bool checkInRange(Particle b);
};

#endif