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

### 3D stencil - Cubes

| samples | sequential | 8 ranks (non-blocking) | 8 ranks (blocking) |
| -: | -: | -: | -: |
| 32 | < 1 second | < 1 second | < 1 second |
| 64 | 6 seconds | 10 seconds | 10 seconds |
| 128 | 95 seconds | 138 seconds | 139 seconds |
| 256 | 1429 seconds | 1782 seconds | 1769 seconds |
