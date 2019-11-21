CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = nbody2d_seq nbody2d_omp

.PHONEY: all
all: nbody2d_seq nbody2d_omp

nbody2d_seq: nbody2d_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o nbody2d_seq

nbody2d_omp: nbody2d_omp.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o nbody2d_omp

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
