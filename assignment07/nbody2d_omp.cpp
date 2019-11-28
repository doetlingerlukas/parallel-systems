#include "../shared/nbody.hpp"

#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char **argv){

  auto start_time = chrono::high_resolution_clock::now();

  //room size
  int Nx = 1000;
  int Ny = 1000;

  //number of particles
  int N = 100;
  bool print = false;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
    if (argc > 2) {
      string verbose = argv[2];
      print = verbose == "--verbose";
    }
  }

  int timesteps = 1000;
  srand(42);

  // initialize particles (randomly)
  vector<Particle> particles;
  particles.reserve(N);
  for (auto i = 0; i < N; i++){
    particles.emplace_back(Nx, Ny, 0);
  }
  vector<Particle> buffer = particles;

  for (auto t = 0; t < timesteps; t++) {
    // at each timestep, calculate the forces between each pair of particles

    #pragma omp parallel for schedule(static, 100)
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if(j < i){
          buffer[i].updateForce(buffer[j]);
        }
        else if (i > j) {
          buffer[i].calculateForce(particles[j]);
        }
      }
      buffer[i].update(Nx, Ny);
    }
    particles = buffer;

    if (print) {
      cout << "timestep :" << t << endl;
      printParticleVector2D(particles, N, Nx, Ny);
      this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
  // time measurement
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration<double>(end_time - start_time).count();
  cout << endl;
  cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}
