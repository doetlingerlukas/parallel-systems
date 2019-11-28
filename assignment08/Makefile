CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = pi_omp

.PHONEY: all
all: pi_omp

pi_omp: pi_omp.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o pi_omp

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
