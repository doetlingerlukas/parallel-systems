# Assignment 9, due December 11th 2019

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

This exercise is about a parallel implementation of the n-queens problem.

### Parallel approach

Since this is a classic backtracking algorithm we can't just use `omp parallel for` and call it a day.
Therefore we used the following OpenMP constructs:
 - `omp parallel`
 - `omp single`
 - `omp task`
 - `omp critical`

The idea is to start the first step of solving the problem with `omp single` inside a parallel block. Within the solving function, we use recursive calls with `omp task`, so other threads also start solving. Hence the recursive calls will be executed in parallel.

When printing a solution, we use `omp critical`, so only one thread will print at a time.

### Potential optimizations

