#!/bin/bash

program="nbody2d_omp"

for i in {1..2}
do
  qsub -sync yes -pe openmp $i job.sh -n $i -p $program
  echo "Result for $i thread(s): " >> benchmark.dat
  cat nbody.dat | grep -o '\d+(\.\d+)?'
  rm -f nbody.dat
done

cat benchmark.dat