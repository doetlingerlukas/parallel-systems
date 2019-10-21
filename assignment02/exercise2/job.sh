#!/bin/bash

while [[ $# -gt 0 ]]
do 
  key="$1"
  case $key in 
    -n|--mpi-n)
      processes="$2"
      shift
      shift
      ;;
    -p|--program)
      program="$2"
      shift
      shift
      ;;
  esac
done

#$ -q std.q
#$ -cwd
#$ -N heat_stencil
#$ -o heat.dat
#$ -j yes

module load gcc/8.2.0
module load openmpi/4.0.1

mpiexec -n $processes --mca btl tcp,self $program