CC = mpicc
CPP = gcc
CPP_FLAGS = -O2 -std=c11 -Wall -Wextra -pedantic

TARGETS = heat_stencil_mpi_v2

.PHONEY: all
all: heat_stencil_mpi_v2

heat_stencil_mpi_v2: heat_stencil_1D_mpi_v2.c
	$(CC) $(CPP_FLAGS) $< -o heat_stencil_mpi_v2

.PHONEY: clean
clean:
	$(RM) $(TARGETS)

.PHONEY: run
run: heat_stencil_seq
	@echo "Sequential:"
	@./heat_stencil_seq
	@echo