CC=gcc
CC_FLAGS=-O3 -g3 -lm -lprofiler 
OMP_FLAGS = -fopenmp

.PHONEY: all
all: real_omp

SRC=$(wildcard *.c)

real_omp: $(SRC)
	@$(CC) $(CC_FLAGS) $(OMP_FLAGS) $(SRC) -o real_omp -lm

.PHONEY: clean
clean:
	@rm real_omp

.PHONEY: run
run: all
	@echo "Parallel:"
	@./real_omp
	@echo
