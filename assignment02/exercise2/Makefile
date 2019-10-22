CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = heat_stencil_seq heat_stencil_mpi

.PHONEY: all
all: heat_stencil_seq heat_stencil_mpi

.PHONEY: seq
seq: heat_stencil_seq

heat_stencil_seq: heat_stencil_1D_seq.cpp
	$(CC) $(CPP_FLAGS) $< -o heat_stencil_seq

heat_stencil_mpi: heat_stencil_1D_mpi.cpp
	$(CC) $(CPP_FLAGS) $< -o heat_stencil_mpi

.PHONEY: clean
clean:
	$(RM) $(TARGETS)

.PHONEY: run
run: heat_stencil_seq
	@echo "Sequential:"
	@./heat_stencil_seq
	@echo
