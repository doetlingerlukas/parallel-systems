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
  int N = 10;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }

  int timesteps = N * 2;

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
    this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
  cout << endl;
  cout << "This took " << duration << " seconds." << endl;

  return EXIT_SUCCESS;
}
