#include "../shared/nbody.hpp"

#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char **argv){

  auto start_time = chrono::high_resolution_clock::now();

  //room size
  int Nx = 20;
  int Ny = 20;

  //number of particles
  int N = 20;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }

  int timesteps = 10;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  mt19937 gen(seed);


  // initialize particles (randomly)
  vector<Particle> particles(N, Particle(0));
  printParticleVector2D(particles, N, Nx, Ny);

  vector<Particle> buffer = particles;

  for (auto t = 0; t < timesteps; t++) {
    // at each timestep, calculate the forces between each pair of particles
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (i != j) {
          buffer[i].calculateForce(particles[j]);
        }
      }
      buffer[i].update();
    }

    particles = buffer;
    cout << "timestep :" << t << endl;

    printParticleVector2D(particles, N, Nx, Ny);

    // sleep to see movement happen
    this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  // time measurement
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
  cout << endl;
  cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}
