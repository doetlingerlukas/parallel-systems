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
#$ -N nbody_2d
#$ -o nbody.dat
#$ -j yes

module load gcc/8.2.0

export OMP_NUM_THREADS=$processes
./$program