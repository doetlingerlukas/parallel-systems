CPP = g++
CPP_FLAGS = -O2 -std=c++17 -Wall -Wextra -pedantic
OMP_FLAGS = -fopenmp

TARGETS = $(sort $(patsubst %.cpp,%,$(wildcard *_omp.cpp)))

.PHONEY: all
all: $(TARGETS)

$(TARGETS): %: %.o
	$(CPP) $(OMP_FLAGS) $(CPP_FLAGS) $< -o $@

.PHONEY: clean
clean:
ifeq ($(OS), Windows_NT)
	del -rf $(patsubst %, %.exe, $(TARGETS)) $(patsubst %, %.o, $(TARGETS))
else
	$(RM) $(TARGETS) $(patsubst %, %.o, $(TARGETS))
endif
