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

Particle getCenterOfMass(vector<Particle> particles){
    Particle p = Particle();
    for (auto i=0; i < particles.size(); i++){
        p.px += particles[i].px;
        p.py += particles[i].py;
        p.vx += particles[i].vx;
        p.vy += particles[i].vy;
        p.fx += particles[i].fx;
        p.fy += particles[i].fy;
    }
    p.px /= particles.size();
    p.py /= particles.size();
    p.vx /= particles.size();
    p.vy /= particles.size();
    p.fx /= particles.size();
    p.fy /= particles.size();
    return p;
}

int main(int argc, char **argv){

  auto start_time = chrono::high_resolution_clock::now();

  //room size
  int N = 1000;

  //number of particles
  int P = 1000;

  int timesteps = 1000;

  if (argc > 1) {
    P = strtol(argv[1], nullptr, 10);
  }

  // MPI init
  int rank_id, number_of_ranks; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_ranks);

  srand(rank_id);

  auto P_rank = P / number_of_ranks;
  auto ranks_per_row = sqrt(number_of_ranks);
  auto N_rank = N / ranks_per_row;

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
    for (auto j = 1; j <= ranks_per_row; j++){
      for (auto k = 1; k <= ranks_per_row; k++){
        local_buffer.emplace_back(N_rank*j, N_rank*k, rank_id); 
      }
    }
  }

  MPI_Datatype MPI_particle;
  int blocklenghts[1] = {6};
  MPI_Aint displacements[1] = {offsetof(Particle_data, vx)};
  MPI_Datatype datatypes[1] = {MPI_DOUBLE};
  MPI_Type_create_struct(1, blocklenghts, displacements, datatypes, &MPI_particle);
  MPI_Type_commit(&MPI_particle);

  vector<Particle> global_buffer(number_of_ranks);
  vector<Particle_data> global_buffer_data(number_of_ranks);

  for (auto t = 0; t < timesteps; t++) {

    // class to struct
    Particle p = getCenterOfMass(local_buffer);
    Particle_data pd = p.toStruct();
    MPI_Allgather(&pd, 1, MPI_particle, &global_buffer_data[0], 1, MPI_particle, MPI_COMM_WORLD);

    // struct to data
    for (int i = 0; i < number_of_ranks; ++i){
      global_buffer[i] = toParticle(global_buffer_data[i]);
    }

    for (int i = 0; i < P_rank; ++i) {
      for (int j = 0; j < P_rank + number_of_ranks; ++j) {
        if (j < number_of_ranks && rank_id != j) {
          local_buffer[i].calculateForce(global_buffer[j]);
        } else if (j >= number_of_ranks) {
          local_buffer[i].calculateForce(local_buffer[j-number_of_ranks]);
        }
      }
      local_buffer[i].update(N, N);
    }
  }

  vector<Particle_data> local_buffer_data(P_rank);
  for (int i = 0; i < P_rank; ++i) {
    local_buffer_data[i] = local_buffer[i].toStruct();
  }

  vector<Particle_data> result_data(P);
  MPI_Allgather(&local_buffer_data[0], P_rank, MPI_particle, &result_data[0], P_rank, MPI_particle, MPI_COMM_WORLD);

  if (rank_id == 0){
    vector<Particle> result(P);
    for (int i = 0; i < P; ++i){
      result[i] = toParticle(result_data[i]);
    }
    printParticleVector2D(result, P, N, N);
  }

  // time measurement
  if (rank_id == 0){
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << endl;
    cout << "This took " << duration << " milliseconds." << endl;
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
