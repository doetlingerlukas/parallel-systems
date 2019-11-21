# Assignment 7

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### Tasks

- Study the OpenMP `parallel` and `for` pragmas and attempt to parallelize the n-body simulation using OpenMP. If you have multiple sequential implementations of n-body, you are free to choose one. Also, study how to submit OpenMP jobs on LCC2.
- Measure the execution time of your OpenMP program for several problem sizes and for 1 to 8 threads.
- Illustrate the data in appropriate speedup/efficiency figures and discuss them. What can you observe?

### Implementation

- ...

### Instructions

- start with ```qsub -sync yes -pe openmp 2 job.sh -n 2 -p 'nbody2d_omp 10000```

### Measurements 

All tests were done on `lcc2` with 1000 timesteps.

threads 8
n
1000    0.528908
2500     2.47735
5000     9.56892
10000   40.3562
25000   226.171

threads 6
n
1000    0.567412
2500    3.40505 
5000    12.6464 
10000   49.5635

threads 4
n
1000    0.776759
2500    4.41846
5000    16.9959
10000   66.6381

threads 2
n
1000    1.25902
2500    7.6515
5000    30.0491
10000   119.275 

threads 1
n
1000    1.73471
2500    11.5217 
5000    46.4206 
10000   186.026 

ranks 64 (mpi 1 opt)
n
25000   104.997

### Figures

![](...)
