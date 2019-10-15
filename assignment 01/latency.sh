#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N osu_latency
#$ -o results/latency.dat
#$ -j yes

module load openmpi/4.0.1

if [ $(whoami) = 'cb761016' ]
then
  mpiexec -display-allocation -display-map /home/cb76/cb761016/a_01/t2/micro-benchmarks/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_latency
elif [ $(whoami) = 'cb761022' ]
then
  mpiexec -display-allocation -display-map /home/cb76/cb761022/micro_benchmarks/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_latency
else 
  # TODO: enter path to exe for third user
  whoami
fi