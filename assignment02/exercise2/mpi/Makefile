CC=gcc
CC_FLAGS=-O2 -std=c99 -Wall -Wextra -pedantic

.PHONEY: all
all: heat_stencil_1D_mpi

heat_stencil_1D_mpi: heat_stencil_1D_mpi.c
	@$(CC) $(CC_FLAGS) $< -o heat_stencil_1D_mpi

.PHONEY: clean
clean:
	@rm heat_stencil_1D_mpi

.PHONEY: run
run: all
	@echo "Parallel:"
	@./heat_stencil_1D_mpi
	@echo