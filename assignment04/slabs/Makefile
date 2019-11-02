CC = mpicc
CPP = mpic++
CPP_FLAGS = -O2 -std=c++11 -Wall -Wextra -pedantic

TARGETS = 3D_slabs_mpi

.PHONEY: all
all: 3D_slabs_mpi

3D_slabs_mpi: 3D_slabs_mpi.cpp
	$(CPP) $(CPP_FLAGS) $< -o 3D_slabs_mpi


.PHONEY: clean
clean:
	$(RM) $(TARGETS)
