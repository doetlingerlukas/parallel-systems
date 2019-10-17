# Assignment 1

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why?

We thought of the simplest approach would be using the `MPI_Reduce` function. `MPI_Reduce` is able to collect values from the ranks and aggregate them. Using `MPI_SUM` as an aggregation function, this is exactly what we need in this case. We collect the values that are outside of the circle at each node, sum them up, and in the root rank we then calculate *pi* by dividing the collected value with the overall sample number (and multiplying by 4).

Our implementation was influenced by following tutorial: https://www.olcf.ornl.gov/tutorials/monte-carlo-pi/.

### Effects and implications of parallelization

## Exercise 2