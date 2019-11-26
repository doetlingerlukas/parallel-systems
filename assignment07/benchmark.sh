#!/bin/bash

program="nbody2d_omp"
prog_file="nbody.dat"
result_file="result.dat"

if [ ! -z "$1" ] ; then
  program+=" ${1}"
fi

# remove result file if it already exists
if [ -f $result_file ] ; then
    rm -f $result_file
fi

# submit jobs and save results
for i in {1..8}
do
  qsub -sync yes -pe openmp $i job.sh -n $i -p $program
  echo "Running the program with $i thread(s): " >> $result_file
  sed -e '$!d' $prog_file >> $result_file
  echo " " >> $result_file
  rm -f $prog_file
done

# print the results
clear
cat $result_file