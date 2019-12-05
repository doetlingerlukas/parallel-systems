CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = n_queens

.PHONEY: all
all: n_queens

matrix: n_queens.cpp
	$(CPP) $(CPP_FLAGS) $(OMP_FLAGS) $< -o n_queens

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
