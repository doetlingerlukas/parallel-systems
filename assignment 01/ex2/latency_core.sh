#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N osu_latency_core
#$ -o results/latency_core.dat
#$ -j yes
#$ -pe openmpi-2perhost 2

module load openmpi/4.0.1
mpiexec -n 2 --map-by core /scratch/c703429/osu-benchmark/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_latency