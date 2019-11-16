#include "../shared/nbody.hpp"

#include <time.h>
#include <chrono>
#include <thread>
#include <mpi.h>
#include <cmath>
#include <cassert>

using namespace std;

Particle toParticle(Particle_data data){
  Particle p = Particle();
  p.px = data.px;
  p.py = data.py;
  p.fx = data.fx;
  p.fy = data.fy;
  p.vx = data.vx;
  p.vy = data.vy;
  return p;
}

int main(int argc, char **argv){

  auto start_time = chrono::high_resolution_clock::now();

  //room size
  int N = 20;

  //number of particles
  int P = 12;

  int timesteps = 20;

  if (argc > 1) {
    N = strtol(argv[1], nullptr, 10);
  }

  // MPI init
  int rank_id, number_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_ranks);

  srand(rank_id);

  auto P_rank = P / number_of_ranks;

  // split problem size among ranks
  if (fmod(sqrtf(number_of_ranks), 1) != 0) {
    if (rank_id == 0) {
      cout <<  "Rank size must be a square number!" << endl;
    }
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  // initialize particles (randomly)
  vector<Particle> local_buffer;
  local_buffer.reserve(P_rank);
  for (auto i = 0; i < P_rank; i++){
    local_buffer.emplace_back(N, N); 
  }

  MPI_Datatype MPI_particle;
  int blocklenghts[1] = {6};
  MPI_Aint displacements[1] = {offsetof(Particle_data, vx)};
  MPI_Datatype datatypes[1] = {MPI_DOUBLE};
  MPI_Type_create_struct(1, blocklenghts, displacements, datatypes, &MPI_particle);
  MPI_Type_commit(&MPI_particle);

  vector<Particle> global_buffer(P);
  vector<Particle_data> global_buffer_data(P);

  for (auto t = 0; t < timesteps; t++) {

    // class to struct
    vector<Particle_data> local_buffer_data(P_rank);
    for (int i = 0; i < P_rank; ++i) {
      local_buffer_data[i] = local_buffer[i].toStruct();
    }

    MPI_Allgather(&local_buffer_data[0], P_rank, MPI_particle, &global_buffer_data[0], P_rank, MPI_particle, MPI_COMM_WORLD);

    // struct to data
    for (int i = 0; i < P; ++i){
      global_buffer[i] = toParticle(global_buffer_data[i]);
    }

    for (int i = 0; i < P_rank; ++i) {
      for (int j = 0; j < P; ++j) {
        if (i + (rank_id * P_rank) != j) {
          local_buffer[i].calculateForce(global_buffer[j]);
        }
      }
      local_buffer[i].update(N, N);
    }

    if (rank_id == 0){
      cout << "timestep :" << t << endl;
      printParticleVector2D(global_buffer, P, N, N);
    }
  }

  // time measurement
  if (rank_id == 0){
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
    cout << endl;
    cout << "This took " << duration << " seconds." << endl;
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
