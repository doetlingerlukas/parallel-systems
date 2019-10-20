CPP=g++
CPP_FLAGS=-O2 -std=c++17 -Wall -Wextra -pedantic

.PHONEY: all
all: heat_stencil_1D_seq

heat_stencil_1D_seq: heat_stencil_1D_seq.cpp
	@$(CPP) $(CPP_FLAGS) $< -o heat_stencil_1D_seq

.PHONEY: clean
clean:
	@rm heat_stencil_1D_seq

.PHONEY: run
run: all
	@echo "Sequential:"
	@./heat_stencil_1D_seq
	@echo