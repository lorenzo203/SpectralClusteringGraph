# Makefile for SpectralClusteringGraph
# Usage:
#   make                -> build the 'challenge2' binary
#   make mkEigenInc=/path/to/eigen3/include -> override Eigen include path
#   make clean          -> remove built files

CXX := g++
# Default Eigen include path; can be overridden on the make command line or via environment
mkEigenInc ?= /usr/include/eigen3

CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I$(mkEigenInc) 

SRC := challenge2.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := challenge2

.PHONY: all clean help

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)

help:
	@echo "Makefile for SpectralClusteringGraph"
	@echo "Targets:"
	@echo "  make                 Build $(TARGET)"
	@echo "  make mkEigenInc=DIR  Build with alternate Eigen include directory"
	@echo "  make clean           Remove build artifacts"
