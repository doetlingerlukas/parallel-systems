# Assignment 7

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### Tasks

- Study the OpenMP `parallel` and `for` pragmas and attempt to parallelize the n-body simulation using OpenMP. If you have multiple sequential implementations of n-body, you are free to choose one. Also, study how to submit OpenMP jobs on LCC2.
- Measure the execution time of your OpenMP program for several problem sizes and for 1 to 8 threads.
- Illustrate the data in appropriate speedup/efficiency figures and discuss them. What can you observe?

### Implementation

To parallelize our n-body application we add the statement `#pragma omp parallel for private(j) schedule(static, 100)` (line 43) to our n-body implementation of assignment 6. While the *explicitly* written down line seems to be very simple, *impliclitly* there is going on much here. At first the directive `pragma omp parallel for` is some syntactic sugar, which opens a `parallel` region that contains a single `for` directive (so you save one line + curly brackets for the parallel region). The iteration variable of the inner loop, `j`, is declared private, meaning that each thread has a local copy of that variable. The iteration variable of the outer loop, `i`, is private per default (defined by the `for` directive). Variables declared outside the scope of the parallel region are shared per default in the parallel region. The `schedule` clause is used for distributing the data more evenly across the threads. If there are `T` threads, then in the default case the first rank gets the first `N/T` particles and so on. If static scheduling is used, the data is distributed based on the schedule chunk size (100 in our case). The first thread takes the first 100 elements, the second thread the next 100 elements and so on. This kind of scheduling is beneficial for our case because we do more computation in the first half of the iteration (in the second half we just flip the sign in the computation). We compared the default scheduling with static scheduling in our measurements below.

### Instructions

Our parallelized application of the n-body problem can be executed on LCC2 with the following line:

```
qsub -sync yes -pe openmp T job.sh -n T -p 'nbody2d_omp N [--verbose]'
```

where `T` represents the number of threads that should be assigned to the job, and `N` represents how many particles should be generated. If `---verbose` is specified, the particles are printed at every timestep.

### Measurements 

We provide a small benchmark via the shell script `benchmark.sh`. For running a benchmark execute:

```
./benchmark.sh 'nbody2d_omp N'
```
where `N` represents the number of particles.

All tests were done on LCC2 with 1000 timesteps.

**default scheduling**

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

**static scheduling:**

threads 8
n
1000    0.55
2500    2.28
5000    8.60
10000   28.1703
25000   169.32

threads 7
n
1000    0.55
2500    2.39
5000    8.16
10000   32.1686 

threads 6
n
1000    0.56
2500    2.78
5000    9.83
10000   36.7641  

threads 5
n
1000    0.56
2500    3.05
5000    11.57
10000   43.4891  

threads 4
n
1000    0.68
2500    3.49
5000    13.72
10000   53.0103   

threads 3
n
1000    0.81
2500    4.43
5000    17.01
10000   71.0239 

threads 2
n
1000    1.03
2500    6.22
5000    23.9
10000   98.2047  

threads 1
n
1000    1.73
2500    11.52
5000    46.32
10000   195.658

ranks 64 (mpi 1 opt)
n
25000   104.997

### Figures

![](...)
