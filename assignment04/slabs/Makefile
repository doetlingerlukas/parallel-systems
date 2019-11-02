CC = mpicc
CPP = mpic++
CPP_FLAGS = -O2 -std=c++11 -Wall -Wextra -pedantic

TARGETS = 3D_slabs_mpi_nonblock 3D_slabs_mpi_block

.PHONEY: all
all: 3D_slabs_mpi_nonblock 3D_slabs_mpi_block

3D_slabs_mpi_nonblock: 3D_slabs_mpi_nonblock.cpp
	$(CPP) $(CPP_FLAGS) $< -o 3D_slabs_mpi_nonblock

3D_slabs_mpi_block: 3D_slabs_mpi_block.cpp
	$(CPP) $(CPP_FLAGS) $< -o 3D_slabs_mpi_block

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
