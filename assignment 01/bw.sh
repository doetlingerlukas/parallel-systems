#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N osu_bw
#$ -o ./results/bw.dat
#$ -j yes
#$ -pe openmpi-2perhost 8

module load openmpi/4.0.1
mpiexec -n 8 ./osu_bw