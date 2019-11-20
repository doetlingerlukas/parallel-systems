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

We have four (working) implementations:
- `nbody_seq`, a sequential version
- `nbody_seq_opt`, a sequential optimized version
- `nbody_mpi_1`, an unoptimized mpi version
- `nbody_mpi_1_opt`, the optimized mpi version

### Measurements 

All tests were done on `lcc2` with 1000 timesteps.

| particles | sequential [s] | seq_opt [s] | mpi (4 ranks) [s] | 4 mpi_opt (4 ranks) [s] | mpi (16 ranks) [s] | mpi opt (16 ranks) |
| -: | -: | -: | -: | -: | -: | -: |
| 500 | 8.3 | 2.4 | 2.7 | 0.7 | 1.7 | 1.4 |
| 1000 | 32.1 | 9.5 | 8.3 | 2.2 | 4.1 | 2.9 |
| 1500 | 71.8 | 21.3 | 18.5 | 4.9 | 5.9 | 3.1 |
| 2000 | 127.7 | 37.7 | 32.6 | 8.4 | 9.8 | 4.5 |
| 2500 | 199.3 | 59 | 50.6 | 17.8 | 15.9 | 6.2 |
