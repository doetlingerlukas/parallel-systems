#!/bin/bash

while [[ $# -gt 0 ]]
do 
  key="$1"
  case $key in 
    -mb|--map-by)
      mapping="--map-by $2"
      shift
      shift
      ;;
  esac
done

#$ -q std.q
#$ -cwd
#$ -N osu_bw
#$ -o bw.dat
#$ -j yes

module load openmpi/4.0.1

if [ $(whoami) = 'cb761016' ]
then
  mpiexec $mapping -display-allocation -display-map /home/cb76/cb761016/a_01/t2/micro-benchmarks/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_bw
elif [ $(whoami) = 'cb761022' ]
then
  mpiexec $mapping -display-allocation -display-map /home/cb76/cb761022/micro_benchmarks/libexec/osu-micro-benchmarks/mpi/pt2pt/osu_bw
else 
  # TODO: enter path to exe for third user
  whoami
fi