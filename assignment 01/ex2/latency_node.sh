#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N osu_latency_node
#$ -o results/latency_node.dat
#$ -j yes
#$ -pe openmpi-2perhost 2

module load openmpi/4.0.1
mpiexec -n 2 --map-by node /scratch/c703429/osu-benchmark/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_latency