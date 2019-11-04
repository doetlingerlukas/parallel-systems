CXX = mpic++
CXX_FLAGS = -O2 -std=c++11 -Wall -Wextra -pedantic

TARGETS = 3D_cubes_mpi_block

.PHONEY: all
all: 3D_cubes_mpi_block

3D_cubes_mpi_block: 3D_cubes_mpi_block.cpp
	$(CXX) $(CXX_FLAGS) $< -o 3D_cubes_mpi_block

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
