# Assignment 10, due January 8th 2020

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

The goal of this assignment is to write a hybrid MPI/OpenMP application.

## Exercise 1

### Tasks

- Implement a hybrid MPI/OpenMP parallel version of the 2D heat stencil.
- Use at least two specific performance optimizations (one for intra-node performance, one for inter-node performance) that were discussed in the lecture so far.
- Benchmark your problem for several problem sizes, numbers of threads and numbers of nodes. Include experiments with all nodes/cores. What can you observe?
- For at least one larger problem size / number of threads / number of nodes combination, benchmark and compare all four cases of a) no optimization b) MPI optimization only c) OpenMP optimization only d) both. What can you observe?

### Hybrid Job Submission

- https://wiki.uiowa.edu/display/hpcdocs/Advanced+Job+Submission
