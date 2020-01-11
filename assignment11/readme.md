# Assignment 11, due January 15th 2020

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

The goal of this assignment is to parallelize an unknown application using profiling and OpenMP.

## Exercise 1

Use tools like **gprof**, **gperftools**, **perf** (not working with WSL :( <-- Therefore, for God's sake, you should use **Linux** ;) ), etc. to analyze the program real_omp. Based on the results, optimize the program with OpenMP.

### gprof

GNU profiler gprof tool uses a hybrid of instrumentation and sampling. Instrumentation is used to collect function call information, and sampling is used to gather runtime profiling information [[1]](https://euccas.github.io/blog/20170827/cpu-profiling-tools-on-linux.html).

- available with GCC
- compile with debug symbols (-g) and gprofsupport (-pg)
- run binary as usual
- run gprofbinary gmon.out to view results
- use --lineto get more detailed, line-based results
    - in our example:  ```gprof real_omp gmon.out --line```
    - output (WSL...Linux has better output):   

Windows output:

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

Linux output:
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  8.92      0.53     0.53                             resid (real_omp.c:534 @ 5d20)
  4.59      0.80     0.27                             resid (real_omp.c:538 @ 5ee6)
  3.91      1.03     0.23                             resid (real_omp.c:532 @ 5d2b)
  3.06      1.21     0.18                             resid (real_omp.c:531 @ 5d00)
  3.06      1.39     0.18                             resid (real_omp.c:534 @ 5d42)
  3.06      1.57     0.18                             vranlc (randdp.c:135 @ b47a)
  2.89      1.74     0.17                             resid (real_omp.c:546 @ 5ef5)
  2.80      1.90     0.17                             resid (real_omp.c:545 @ 5efb)
  2.72      2.06     0.16                             vranlc (randdp.c:136 @ b48e)
  2.72      2.22     0.16                             vranlc (randdp.c:132 @ b451)
  2.55      2.37     0.15                             comm3 (real_omp.c:877 @ 309f)
  2.38      2.51     0.14                             psinv (real_omp.c:472 @ 68d7)
  2.29      2.65     0.14                             resid (real_omp.c:532 @ 5d0c)
  2.21      2.78     0.13                             resid (real_omp.c:537 @ 5f2f)
  ...
  ```

### gpertools

- Tutorial for WSL: https://github.com/usnistgov/OOF2/issues/6 (Step 3)
- link with –lprofiler
- run with environment variable CPUPROFILE=prof.out
- run pprofbinary prof.out to view results (--gv for graphical visualization)

### perf

Uses CPU performance counters to profile (sampling tool).

* How to use:
```
perf record ./real_omp  # creates file perf.data
perf report
```
This gives following output:
```
Overhead  Command   Shared Object          Symbol
  28.60%  real_omp  real_omp               [.] resid
  19.45%  real_omp  libgomp.so.1.0.0       [.] gomp_team_barrier_wait_end
  12.14%  real_omp  real_omp               [.] psinv
  11.04%  real_omp  real_omp               [.] vranlc
  10.06%  real_omp  libgomp.so.1.0.0       [.] gomp_barrier_wait_end
   6.72%  real_omp  real_omp               [.] interp
   6.31%  real_omp  real_omp               [.] rprj3
```


### References

* https://euccas.github.io/blog/20170827/cpu-profiling-tools-on-linux.html

### Description

The file [real.tar.gz](real.tar.gz) contains a realistic implementation of a (simple) numerical algorithm. Imagine you are tasked with making this implementation faster by parallelizing it with OpenMP, without any further information.

### Tasks

- Familiarize yourself with the code. You are not required to look at every source line, but rather profile the code using the means discussed in the lecture and get a grasp on its computational hotspots and performance characteristics (computation-heavy, memory-heavy, etc.).
- Investigate any loops that carry larger workloads and determine if and how they can be parallelized. Parallelize them with OpenMP. Ensure that any code modification does not violate program correctness with respect to its output.
- Benchmark the original, sequential program and your parallelized version for 1, 2, 4 and 8 threads on LCC2 and enter your results in [this table](https://docs.google.com/spreadsheets/d/1hLTIc-VlzBOBrlZY2cSt1RIKc376UYyOLge2QcnJ7sQ/edit?usp=sharing).
