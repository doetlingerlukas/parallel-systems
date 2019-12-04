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

| N | seq [s] | OpenMP 2 [ms] | OpenMP 4 [ms] | OpenMP 8 [ms] |
| -: | -: | -: | -: | -: | 
| 10^6 | 37 ms | 8 ms | 4 ms | 2 ms |
| 10^7 | 162 ms | 81 ms | 41 ms | 35 ms |
| 10^8 | 1622 ms | 816 ms | 407 ms  | 212 ms |
| 10^9 | 16252 ms | 8151 ms | 4069 ms | 2049 ms |


### 2D heat stencil with OpenMP

 Our first intention to parallelize this program with OpenMP was to use the `collapse` clause on the OpenMP `for` directive, to split up the two nested for loops that iterate over the 2D array, representing our room. This approach gave us good speedup compared to the sequential one, but it was slower compared to an OpenMP version that uses just the `for` directive (without collapse). To optimize the application further, we tried to distribute the workload with several scheduling algorithms, but because the needed computational power is more or less evenly distributed (because of random initialization) over the iterations, no scheduling algorithm improved the performance. Last, we tried to move the parallel region above the outer loop, so that the region is not initialized at every timestep. It seems that the compiler already makes this optimization, because we got no speedup.

#### Measurements

| N | seq [s] | OpenMP 2 [s] | OpenMP 4 [s] | OpenMP 8 [s]
| -: | -: | -: | -: | -: |
| 200 | 4 | 2 | 1 | 0 |
| 300 | 14 | 7 | 3 | 2 |
| 500 | 66 | 33 | 16 | 9 |
| 700 | 195 | 91 | 46 | 23 |

## Exercise 2

The loop, which calculates a value in the new matrix, has been parallelized with `omp parallel for`. In addition to that, we sum up the subrusults of a new matrix-element in an additional variable and add it to the resulting matrix at the end. This reduces updates on the result matrix to a minimum, which is ideal, since the result matrix is shared among all threads.

### Measurements

| N | seq [s] | OpenMP 2 [s] | OpenMP 4 [s] | OpenMP 8 [s]
| -: | -: | -: | -: | -: |
| 500 | 0.68 | 0.35 | 0.2 | 0.12 |
| 1000 | 8.87 | 4.78 | 2.4 | 1.39 |
| 1500 | 31.9 | 16.29 | 8.83 | 5.07 |
| 2000 | 78 | 40 | 21.74 | 12.66 |
| 2500 | 150 | 81.6 | 44.5 | 25.29 |
