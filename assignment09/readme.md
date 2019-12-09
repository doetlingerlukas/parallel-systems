# Assignment 9, due December 11th 2019

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

This exercise is about the n-queen problem. Goal is to provide a sequential and a parallelized version with OpenMP, do some benchmarks and think of additional optimizations.

### Sequential version

We implemented the n-queens problem with a technique called backtracking. Backtracking algorithms compute solutions by taking previous steps into account. The idea is to place a queen on the board, and then search for the next valid position for the next queen. 

We also implemented a naive brute force algorithm for comparison, which does not only compute valid solutions, but instead, generates all possibilites. Afterwards we only print the subset valid solutions.

### Parallel version

Since this is a classic backtracking algorithm we can't just use `omp parallel for` and call it a day.
Therefore we used the following OpenMP constructs:
 - `omp parallel`
 - `omp single`
 - `omp task`
 - `omp critical`

The idea is to start the first step of solving the problem with `omp single` inside a parallel block. Within the solving function, we use recursive calls with `omp task`, so other threads also start solving. Hence the recursive calls will be executed in parallel.

When printing a solution, we use `omp critical`, so only one thread will print at a time.

### Potential optimizations


### Resources 

* https://rosettacode.org/wiki/N-queens_problem#C
* https://stackoverflow.com/questions/3184893/use-next-permutation-to-permutate-a-vector-of-classes
