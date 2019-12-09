CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = n_queens n_queens_naive n_queens_omp

.PHONEY: all
all: n_queens n_queens_naive n_queens_omp

n_queens: n_queens.cpp
	$(CPP) $(CPP_FLAGS) $< -o n_queens

n_queens_naive: n_queens_naive.cpp
	$(CPP) $(CPP_FLAGS) $< -o n_queens_naive

n_queens_omp: n_queens_omp.cpp
	$(CPP) $(CPP_FLAGS) $(OMP_FLAGS) $< -o n_queens_omp

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
