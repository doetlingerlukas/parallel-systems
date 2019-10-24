CPP = g++
CPP_FLAGS = -g -std=c++11 -Wall -Wextra -pedantic

TARGETS = heat_stencil_2D_seq heat_stencil_3D_seq

.PHONEY: seq
seq: heat_2D_seq heat_3D_seq

heat_2D_seq: heat_stencil_2D_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o heat_stencil_2D_seq

heat_3D_seq: heat_stencil_3D_seq.cpp
	$(CPP) $(CPP_FLAGS) $< -o heat_stencil_3D_seq

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
