#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N osu_bw
#$ -o results/bw.dat
#$ -j yes

module load openmpi/4.0.1

if [ $(whoami) = 'cb761016' ]
then
  mpiexec -n 2 /home/cb76/cb761016/a_01/t2/micro-benchmarks/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_bw
elif [ $(whoami) = 'cb' ]
then
  # TODO: enter path to exe for different user + add username in alif statement
  /bin/hostname
else 
  # TODO: enter path to exe for third user
  whoami
fi