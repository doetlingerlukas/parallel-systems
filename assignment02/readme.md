# Assignment 1

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why?

We thought that the simplest approach would be using the `MPI_Reduce` function. `MPI_Reduce` is able to collect values from the ranks and aggregate them. With `MPI_SUM` as an aggregation function, this is exactly what we need in this case. We collect the values that are outside of the circle at each node, sum them up, and in the root rank we then calculate *pi* by dividing the collected value with the overall sample number (and multiplying by 4).

Our implementation was influenced by following tutorial: https://www.olcf.ornl.gov/tutorials/monte-carlo-pi/.

### Accuracy effects of parallelization

| samples | sequential | 2pernode 8 | δ sequential | δ 2pernode 8 |
| -: | -: | -: | -: | -: |
| 10000 | 3.134487 | 3.147200 | 0.0071 | 0.056 |
| 100000 | 3.149609 | 3.140320 | 0.008017 | 0.001272 |
| 1000000 | 3.143745 | 3.141460 | 0.0022 | 0.002153 |
| 10000000 | 3.140680 | **3.141502** | 0.000912 | **0.00009** |

### Time effects of parallelization


## Exercise 2