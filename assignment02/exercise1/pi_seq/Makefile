CC=gcc
CC_FLAGS=-O2 -std=c99 -Wall -Wextra -pedantic

.PHONEY: all
all: pi_seq

heat_stencil_1D_seq: pi_seq.c
	@$(CC) $(CC_FLAGS) $< -o heat_stencil_1D_seq

.PHONEY: clean
clean:
	@rm pi_seq

.PHONEY: run
run: all
	@echo "Sequential:"
	@./pi_seq
	@echo