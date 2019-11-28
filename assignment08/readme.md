# Assignment 8, due December 4th 2019

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

The goal of this assignment is to parallelize several applications with OpenMP.

## Exercise 1

### Monte Carlo Pi with OpenMP

In our original sequential implementation we used *rand()* as a random number generator. But this function is not thread safe.
Using it in multi-thread environment causes a race condition on its hidden state variables, thus lead to poor performance.
So we now use *rand_r* with a different seed for each thread, dependent of its *thread_num*.
Each thread has a local copy of its own *seed*, *x* and *y* values. Using the *reduction* possiblity of OpenMP on the *count* variable parallelizing the code is straight forward. (TODO)

#### Measurements
TODO

## Exercise 2

