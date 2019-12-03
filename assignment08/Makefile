CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = pi_omp heat_stencil_2D_omp

.PHONEY: all
all: pi_omp heat_stencil_2D_omp matrix

pi_omp: pi_omp.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o pi_omp

heat_stencil_2D_omp: heat_stencil_2D_omp.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o heat_stencil_2D_omp

matrix: matrix_mult_omp.cpp
	$(CPP) $(CPP_FLAGS) $(OMP_FLAGS) $< -o matrix_mult_omp

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
