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
#$ -N heat_stencil_2D_hybrid
#$ -o hybrid.dat
#$ -j yes

module load gcc/8.2.0
module load openmpi/4.0.1

export OMP_NUM_THREADS=4

mpiexec --map-by node -n $processes --mca btl self,vader,tcp $program