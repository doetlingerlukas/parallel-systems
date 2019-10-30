CC = mpic++
CPP = g++
CPP_FLAGS = -g -std=c++11 -Wall -Wextra -pedantic

TARGETS = heat_stencil_2D_seq heat_stencil_3D_seq heat_stencil_3D_mpi

.PHONEY: all
all: heat_stencil_2D_seq heat_stencil_3D_seq heat_stencil_2D_mpi heat_stencil_3D_mpi

.PHONEY: seq
seq: heat_2D_seq heat_3D_seq

heat_stencil_2D_seq: heat_stencil_2D_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o heat_stencil_2D_seq

heat_stencil_2D_mpi: heat_stencil_2D_mpi.cpp
	$(CC) $(CPP_FLAGS) $< -o heat_stencil_2D_mpi

heat_stencil_3D_seq: heat_stencil_3D_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o heat_stencil_3D_seq

heat_stencil_3D_mpi: heat_stencil_3D_mpi.cpp
	$(CC) $(CPP_FLAGS) $< -o heat_stencil_3D_mpi

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
