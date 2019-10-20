CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = pi_seq pi_mpi

.PHONEY: all
all: pi_seq pi_mpi

.PHONEY: seq
seq: pi_seq

pi_seq: pi_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o pi_seq

pi_mpi: pi_mpi.c
	$(CC) $(CPP_FLAGS) $< -o pi_mpi

.PHONEY: clean
clean:
	$(RM) $(TARGETS)

.PHONEY: run
run: pi_seq
	@echo "Sequential:"
	@./pi_seq
	@echo