// particle.hpp
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

constexpr double G = 1.0;
constexpr double M = 1.0;
constexpr double THRESHOLD = 5.0;

typedef struct { // struct representation of particle, for sending
  double vx;
  double vy;
  double fx;
  double fy;
  double px;
  double py;
} Particle_data;

class Particle {

  double m = M; // mass

  public:
    double px, py; // position
    double fx, fy; // force
    double vx, vy; // velocity
    int rank_id;

    Particle(){}
    Particle(int Nx, int Ny);
    Particle(int Nx, int Ny, int id);

    void printParticle();
    double getRadius(Particle b);
    void calculateForce(Particle b);
    void updateForce(Particle b);
    void update(int Nx, int Ny);
    bool checkInRange(Particle b);
    Particle_data toStruct();
};

#endif