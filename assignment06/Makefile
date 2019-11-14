CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = nbody2d_seq

.PHONEY: all
all: nbody2d_seq

nbody2d_seq: nbody2d_seq.cpp
	$(CPP) $(CPP_FLAGS) $< particle.cpp -o nbody2d_seq

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
