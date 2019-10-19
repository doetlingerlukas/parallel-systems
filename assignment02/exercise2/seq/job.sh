#!/bin/bash

#Excecute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N pi_seq

# Redirect output stream to this file.
#$ -o ./output.dat

# Join the error stream to the output stream.
#$ -j yes

#$ -pe openmpi-1perhost 1

module load openmpi/4.0.1

mpiexec -n 1 ./heat_stencil_1D_seq
