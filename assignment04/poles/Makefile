CC = mpic++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = 2d_poles 3d_poles

.PHONEY: all
all: 2d_poles 3d_poles

2d_poles: 2d_poles.cpp
	$(CC) $(CPP_FLAGS) $< -o 2d_poles

3d_poles: 3d_poles.cpp
	$(CC) $(CPP_FLAGS) $< -o 3d_poles

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
