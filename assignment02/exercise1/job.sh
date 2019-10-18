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
    -s|--samples)
      samples="$2"
      shift
      shift
      ;;
  esac
done

#$ -q std.q
#$ -cwd
#$ -N pi_calc
#$ -o pi.dat
#$ -j yes

module load openmpi/4.0.1

mpiexec -n $processes $program $samples