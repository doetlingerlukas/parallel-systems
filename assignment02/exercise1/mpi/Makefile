CC=mpicc
CC_FLAGS=-O2 -std=c99 -Wall -Wextra -pedantic

.PHONEY: all
all: pi_mpi

heat_stencil_1D_seq: pi_mpi.c
	@$(CC) $(CC_FLAGS) $< -o pi_mpi

.PHONEY: clean
clean:
	@rm pi_mpi

.PHONEY: run
run: all
	@echo "Parallel:"
	@./pi_mpi
	@echo