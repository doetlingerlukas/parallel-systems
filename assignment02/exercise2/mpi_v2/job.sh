#!/bin/bash

#Excecute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N stencil_mpi

# Redirect output stream to this file.
#$ -o output.dat

# Join the error stream to the output stream.
#$ -j yes

#$ -pe openmpi-2perhost 2

module load openmpi/4.0.1

mpiexec -n 2 /home/cb76/cb761022/parallel-systems/assignment02/exercise2/mpi_v2/heat_stencil_mpi_v2