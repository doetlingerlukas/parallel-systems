# Assignment 8, due December 4th 2019

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

The goal of this assignment is to parallelize several applications with OpenMP.

## Exercise 1

### Monte Carlo Pi with OpenMP

In our original sequential implementation we used *rand()* as a random number generator. But this function is not thread safe (found in https://bit.ly/2DEg2K8).
Using it in multi-thread environment causes a race condition on its hidden state variables, thus lead to poor performance.
So we now use *rand_r* with a different seed for each thread, dependent of its *thread_num*.
Each thread has a local copy of its own *seed*, *x* and *y* values. Using the *reduction* possiblity of OpenMP on the *count* variable parallelizing the code is straight forward.

#### Measurements

|| Threads: 1 | Threads: 2 | Threads: 4 | Threads: 6 | Threads: 8
| -: | -: | -: | -: | -: | -: | -: | -: |
Samples: *10^6*| 21 ms | 22 ms | 22 ms | 22 ms | 22 ms |
Samples: *10^7*| 21 ms | 22 ms | 22 ms | 211 ms | 210 ms |
Samples: *10^8*| 21 ms | 22 ms | 22 ms | 211 ms | 210 ms |

## Exercise 2

