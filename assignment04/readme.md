# Assignment 4

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

A 3D heat stencil was implemented using the following domain decompositions: slabs, poles and cubes.

For the *slabs-* and *poles-* implementation please refer to the equally named subdirectories. The *cubes-* implementation is located at `../assignment03/heat_stencil_3D_mpi.cpp`.

For performance, head to the measurements section.

## Exercise 2

Blocking variants of the programs can be found in the equally named subdirectories.
For performance, head to the measurements section.

## Measurments

### 3D stencil - Slabs

| samples | sequential | 4 ranks (non-blocking) | 8 ranks (non-blocking) |
| -: | -: | -: | -: |
| 32 | 0,807 | 0,567 | 0,524 |
| 64 | 12,885 | 7,462 | 5,646 |
| 72 | 35,282 | 10,849 | 8,110 |
| 80 | 53,723 | 16,486 | 10,958 |

| speedup 8p8 (non-blocking) | efficiency 8p8 (non-blocking) | speedup 4p4 (non-blocking) | efficiency 4p4 (non-blocking)
| -: | -: | -: | -: |
| 1,540 | 0,193	| 1,423	| 0,356
| 2,282	| 0,285	| 1,727	| 0,432
| 4,350	| 0,544	| 3,252	| 0,813
| 4,903	| 0,613	| 3,259	| 0,815


| samples | sequential | 4 ranks (blocking) | 8 ranks (blocking) |
| -: | -: | -: | -: |
| 32 | 0,807 | 0,657 | 0,532
| 64 | 12,885 | 7,698 | 4,964
| 72 | 35,282 | 12,244 | 7,776
| 80 | 53,723 | 16,495 | 10,700

| speedup 8p8 (blocking) | efficiency 8p8 (blocking) | speedup 4p4 (blocking) | efficiency 4p4 (blocking)
| -: | -: | -: | -: |
| 1,517 | 0,190	| 1,228	| 0,307
| 2,596 | 0,324	| 1,674	| 0,418
| 4,537	| 0,567	| 2,882	| 0,720
| 5,021	| 0,628	| 3,257	| 0,814



### 3D stencil - Cubes

| samples | sequential | 8 ranks (non-blocking) | 8 ranks (blocking) |
| -: | -: | -: | -: |
| 32 | < 1 second | < 1 second | < 1 second |
| 64 | 6 seconds | 10 seconds | 10 seconds |
| 128 | 95 seconds | 138 seconds | 139 seconds |
| 256 | 1429 seconds | 1782 seconds | 1769 seconds |


### 3D stencil - Poles

| samples | sequential [s] | 4 ranks (blocking) [s] | 4 ranks (non-blocking) [s] |
| -: | -: | -: | -: |
| 32 | <1 | 1 | 1 | 
| 64 | 12 | 19 | 19 |
| 72 | 22 | 29 | 29 |
| 80 | 34 | 43 | 41 |
| 128 | 221 | 257 | 254 |

| speedup (blocking) | efficiency (blocking) | speedup (non-blocking) | efficiency (non-blocking)
| -: | -: | -: | -: |
| 1 | 0.25 | 1 | 0.25 |
| 0.63 | 0.16 | 0.63 | 0.16 |
| 0.76 | 0.19 | 0.76 | 0.19 |
| 0.79 | 0.20 | 0.83 | 0.21 |
| 0.86 | 0.22 | 0.87 | 0.22 |
