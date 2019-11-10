CC = mpic++
CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = nbody2d

.PHONEY: all
all: nbody2d

nbody2d: nbody2d.cpp
	$(CPP) $(CPP_FLAGS) $< particle.cpp -o nbody2d

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
