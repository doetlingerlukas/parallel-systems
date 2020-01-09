# Assignment 11, due January 15th 2020

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

The goal of this assignment is to parallelize an unknown application using profiling and OpenMP.

## Exercise 1

### Tips from lecture

Use **gprof**, **gperftools** and **perf** (not working with WSL :()!

- gprof:
    - available with GCC
    - compile with debug symbols (-g) and gprofsupport (-pg)
    - run binary as usual
    - run gprofbinary gmon.outto view results
    - use --lineto get more detailed, line-based results
        - in our example:  ```gprof real_omp gmon.out --line```
        - output (WSL...Linux has better output):   

| %    | cumulative | self    | self   | total   |         |                                |
|------|------------|---------|--------|---------|---------|--------------------------------|
| time | seconds    | seconds | calls  | Ts/call | Ts/call | name                           |
| 0.00 | 0.00       | 0.00    | 131642 | 0.00    | 0.00    | randlc (randdp.c:5 @ aa70)     |
| 0.00 | 0.00       | 0.00    | 131072 | 0.00    | 0.00    | vranlc (randdp.c:72 @ ab40)    |
| 0.00 | 0.00       | 0.00    | 493    | 0.00    | 0.00    | setup (real_omp.c:330 @ 1e60)  |
| 0.00 | 0.00       | 0.00    | 168    | 0.00    | 0.00    | psinv (real_omp.c:451 @ 67d0)  |
| 0.00 | 0.00       | 0.00    | 147    | 0.00    | 0.00    | interp (real_omp.c:653 @ 8010) |
| 0.00 | 0.00       | 0.00    | 147    | 0.00    | 0.00    | resid (real_omp.c:513 @ 5a50)  |
| 0.00 | 0.00       | 0.00    | 147    | 0.00    | 0.00    | rprj3 (real_omp.c:572 @ 7520)  |

- gpertools:
    - Tutorial for WSL: https://github.com/usnistgov/OOF2/issues/6 (Step 3)
    - link with –lprofiler
    - run with environment variable CPUPROFILE=prof.out
    - run pprofbinary prof.outto view results (--gvfor graphical visualization)

### Description

The file [real.tar.gz](real.tar.gz) contains a realistic implementation of a (simple) numerical algorithm. Imagine you are tasked with making this implementation faster by parallelizing it with OpenMP, without any further information.

### Tasks

- Familiarize yourself with the code. You are not required to look at every source line, but rather profile the code using the means discussed in the lecture and get a grasp on its computational hotspots and performance characteristics (computation-heavy, memory-heavy, etc.).
- Investigate any loops that carry larger workloads and determine if and how they can be parallelized. Parallelize them with OpenMP. Ensure that any code modification does not violate program correctness with respect to its output.
- Benchmark the original, sequential program and your parallelized version for 1, 2, 4 and 8 threads on LCC2 and enter your results in [this table](https://docs.google.com/spreadsheets/d/1hLTIc-VlzBOBrlZY2cSt1RIKc376UYyOLge2QcnJ7sQ/edit?usp=sharing).
