CXX := g++
CXX_FLAGS := -O2 -std=c++17 -Wall -Wextra -pedantic

TARGETS = $(sort $(patsubst %.cpp,%,$(wildcard *_seq.cpp)))

ifneq ($(OS), Windows_NT)
	CXX = mpic++
	TARGETS += $(sort $(patsubst %.cpp,%,$(wildcard *_mpi.cpp)))
endif

.PHONEY: all
all: $(TARGETS)

$(TARGETS): %: %.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

.PHONEY: clean
clean:
ifeq ($(OS), Windows_NT)
	del -rf $(patsubst %, %.exe, $(TARGETS)) $(patsubst %, %.o, $(TARGETS))
else
	$(RM) $(TARGETS) $(patsubst %, %.o, $(TARGETS))
endif