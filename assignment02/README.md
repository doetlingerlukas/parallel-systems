# Assignment 2, due October 23rd 2019

The goal of this assignment is to implement some basic MPI applications.

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

This exercise consists in writing a parallel application to speed up the computation of π.

### Description

There are many ways of approximating π, one being a well-known Monte Carlo method: The ratio of the areas of a square and its incircle is π/4. Since the exact area of a circle cannot be computed (we don't know the value of π yet), one can instead sample random points, check their distance from the center and compute the ratio of points inside the circle to all sampled points.

<img src="https://upload.wikimedia.org/wikipedia/commons/2/20/MonteCarloIntegrationCircle.svg" width="40%">

### Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why?

We thought that the simplest approach would be using the `MPI_Reduce` function. `MPI_Reduce` is able to collect values from the ranks and aggregate them. With `MPI_SUM` as an aggregation function, this is exactly what we need in this case. We collect the values that are outside of the circle at each node, sum them up, and in the root rank we then calculate *pi* by dividing the collected value with the overall sample number (and multiplying by 4).

Our implementation was influenced by following tutorial: https://www.olcf.ornl.gov/tutorials/monte-carlo-pi/.

### Accuracy effects of parallelization

| samples | sequential | 2pernode 8 | δ sequential | δ 2pernode 8 |
| -: | -: | -: | -: | -: |
| 10.000 | 3.134487 | 3.147200 | 0.0071 | 0.056 |
| 100.000 | 3.149609 | 3.140320 | 0.008017 | 0.001272 |
| 1.000.000 | 3.143745 | 3.141460 | 0.0022 | 0.002153 |
| 10.000.000 | 3.140680 | **3.141502** | 0.000912 | **0.00009** |

### Time effects of parallelization

| samples | sequential | 2pernode 2 | 2pernode 4 | 2pernode 8 |
| -: | -: | -: | -: | -: |
| 10.000 | **1ms** | 1064ms | 846ms | 1360ms |
| 100.000 | **4ms** | 1277ms | 850ms | 1688ms |
| 1.000.000 | **29ms** | 1223ms | 996ms | 1255ms |
| 10.000.000 | **289ms** | 3541ms | 2065ms | 1719ms |
| 100.000.000 | **2931ms** | 15870ms | 13317ms | 6935ms |
| 1.000.000.000 | 83326ms | 50032ms | 49660ms | **24808ms** |

![](img/pi_time.png)
![](img/pi_time_log.png)

## Exercise 2

This exercise consists in parallelizing an application simulating the propagation of heat.

### Description

A large class of scientific applications are so-called stencil applications. These simulate time-dependent physical processes such as the propagation of heat or pressure in a given medium. The core of the simulation operates on a grid and updates each cell with information from its neighbor cells.

<img src="https://upload.wikimedia.org/wikipedia/commons/e/ec/2D_von_Neumann_Stencil.svg" width="40%">

### Tasks

- A sequential implementation of a 1-D heat stencil is available in [heat_stencil_1D_seq.c](heat_stencil_1D/heat_stencil_1D_seq.c). Read the code and make sure you understand what happens. See the Wikipedia article on [Stencil Codes](https://en.wikipedia.org/wiki/Stencil_code) for more information.
- Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why? Are there additional changes required in the code beyond calling MPI functions? If so, elaborate!
- Implement your chosen parallelization strategy as a second application `heat_stencil_1D_mpi`. Run it with varying numbers of ranks and problem sizes and verify its correctness by comparing the output to `heat_stencil_1D_seq`.
- Discuss the effects and implications of your parallelization.

