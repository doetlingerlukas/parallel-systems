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

### 2D heat stencil with OpenMP

Our first intention to parallelize this program with OpenMP was to use the `collapse` clause on the OpenMP `for` directive, to split up the two nested for loops that iterate over the 2D array, representing our room. This approach gave us good speedup compared to the sequential one, but it was slower compared to an OpenMP version that uses just the `for` directive (without collapse). To optimize the application further, we tried to distribute the workload with several scheduling algorithms, but because the needed computational power is more or less evenly distributed (because of random initialization) over the iterations, no scheduling algorithm improved the performance. Last, we tried to move the parallel region above the outer loop, so that the region is not initialized at every timestep. It seems that the compiler already makes this optimization, because we got no speedup.


## Exercise 2

