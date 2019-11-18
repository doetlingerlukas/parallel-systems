# Assignment 6

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### Tasks

- Provide an implementation of your parallelization and optimization plan for the n-body simulation of Assignment 5.
- Measure the speedup and efficiency for multiple problem and machine sizes as in previous exercises.
- Illustrate the data in appropriate figures and discuss them. What can you observe?

### Implementation

For assignment05, we came up with 2 different approaches for parallelization, splitting the area or splitting the problem size.

We tried to split the computation area, which ended up in a *communication-overhead-mess*. Therefore we devided the problem size among the ranks.

We have three implementations:
- `nbody_seq`, a sequential version
- `nbody_mpi`, an unoptimized mpi version
- `nbody_mpi_2`, the optimized mpi version

### Measurements 

All tests were done on `lcc2` with 1000 timesteps.

| particles | sequential [s] | mpi_basic (4 ranks) [s] | mpi_advanced (4 ranks) [s] | mpi_advanced (16 ranks) [s] |
| -: | -: | -: | -: | -: |
| 500 | 2.39 | 2.75 | 0.62 | 0.33 |
| 1000 | 8.31 | 8.86 | 2.15 | 0.44 |
| 2000 | 31.54 | 33.33 | 8.23 | 0.87 |
| 5000 | 192.64 | 203.14 | 50.97 | 3.66 |
