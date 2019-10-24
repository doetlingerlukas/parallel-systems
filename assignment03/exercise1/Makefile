CC = mpic++
CPP = g++
CPP_FLAGS = -g -std=c++11 -Wall -Wextra -pedantic

TARGETS = heat_stencil_2D_seq

.PHONEY: seq
seq: pi_seq

pi_seq: heat_stencil_2D_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o heat_stencil_2D_seq

.PHONEY: clean
clean:
	$(RM) $(TARGETS)

.PHONEY: run
run: heat_stencil_2D_seq
	@echo "Sequential:"
	@./heat_stencil_2D_seq
	@echo