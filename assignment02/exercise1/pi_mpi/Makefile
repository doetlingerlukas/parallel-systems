CC=gcc
CC_FLAGS=-O2 -std=c99 -Wall -Wextra -pedantic

.PHONEY: all
all: pi_mpi

pi_mpi: pi_mpi.c
	@$(CC) $(CC_FLAGS) $< -o pi_mpi

.PHONEY: clean
clean:
	@rm pi_mpi

.PHONEY: run
run: all
	@echo "Sequential:"
	@./pi_mpi
	@echo