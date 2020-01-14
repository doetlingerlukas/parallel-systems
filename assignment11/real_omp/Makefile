CC=gcc
CC_FLAGS=-O2 -lm -std=c99 -march=native -mtune=native -ftree-vectorize -ftree-vectorizer-verbose=1
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
