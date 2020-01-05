CPP = mpic++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = heat_stencil_2D_hybrid heat_stencil_2D_seq heat_stencil_2D_hybrid_non_opt heat_stencil_2D_hybrid_rma

.PHONEY: all
all: heat_stencil_2D_hybrid heat_stencil_2D_seq heat_stencil_2D_hybrid_rma heat_stencil_2D_hybrid_non_opt

heat_stencil_2D_hybrid: heat_stencil_2D_hybrid.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o heat_stencil_2D_hybrid

heat_stencil_2D_hybrid_non_opt: heat_stencil_2D_hybrid_non_opt.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o heat_stencil_2D_hybrid_non_opt

heat_stencil_2D_hybrid_rma: heat_stencil_2D_hybrid_rma.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o heat_stencil_2D_hybrid_rma

heat_stencil_2D_seq: heat_stencil_2D_seq.cpp
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o heat_stencil_2D_seq

.PHONEY: clean
clean:
	$(RM) $(TARGETS)
