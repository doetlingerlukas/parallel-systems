#include <iostream>
#include <cmath>
#include <cassert>
#include <random>

using namespace std;

constexpr double G = 1.0;
constexpr double M = 1.0;
constexpr double THRESHOLD = 5.0;

// struct representation of particle, for sending
typedef struct {
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

    Particle(int id) {
      random_device rd;
      mt19937 gen(rd());
      uniform_real_distribution<double> dis(0.0, 1.0);

      px = dis(gen);
      py = dis(gen);
      vx = dis(gen);
      vy = dis(gen);
      rank_id = id;
    }

    void printParticle() {
      cout << "position: (" << px << ", " << py << ")" <<endl;
    }

    double getRadius(Particle b) {
      double dx = px - b.px;
      double dy = py - b.py;
      double r = sqrt(dx*dx + dy*dy);
      return r;
    }

    void calculateForce(Particle b) {
      double rad = getRadius(b);
      //assert(rad > 0.0);
      if (!(rad > 0.0)){
        rad = 0.001;
      }

      double F = G * (m * b.m) / (rad * rad);
      fx = F * (px - b.px) / rad;
      fy = F * (py - b.py) / rad;
    }


    void updateForce(Particle b) {
      fx = -b.fx;
      fy = -b.fy;
    }

    void update() {
      vx += fx / m;
      vy += fy / m;
      px += vx;
      py += vy;
      
      // handle egde cases, flip velocity sign if on border
      if (px < 0.0){
        px = 0.0;
        vx = -vx;
      } else if (px > 1.0){
        px = 1.0;
        vx = -vx;
      }
      if (py < 0.0){
        py = 0.0;
        vy = -vy;
      } else if (py > 1.0){
        py = 1.0;
        vy = -vy;
      }
    }

    bool checkInRange(Particle b) {
      if(getRadius(b) < THRESHOLD){
        return true;
      }
      return false;
    }

    Particle_data toStruct() {
      Particle_data p = {vx, vy, fx, fy, px, py};
      return p;
    }
};