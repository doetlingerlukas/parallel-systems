CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = nbody2d_seq nbody2d_seq_opt nbody2d_mpi_1 nbody2d_mpi_1_opt nbody2d_mpi_2

.PHONEY: all
all: nbody2d_seq nbody2d_seq_opt nbody2d_mpi_1 nbody2d_mpi_1_opt nbody2d_mpi_2

nbody2d_seq: nbody2d_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o nbody2d_seq

nbody2d_seq_opt: nbody2d_seq_opt.cpp
	$(CPP) $(CPP_FLAGS) $< -o nbody2d_seq_opt

nbody2d_mpi_1: nbody2d_mpi_1.cpp
	$(CC) $(CPP_FLAGS) $< -o nbody2d_mpi_1

nbody2d_mpi_1_opt: nbody2d_mpi_1_opt.cpp
	$(CC) $(CPP_FLAGS) $< -o nbody2d_mpi_1_opt

nbody2d_mpi_2: nbody2d_mpi_2.cpp
	$(CC) $(CPP_FLAGS) $< -o nbody2d_mpi_2

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
