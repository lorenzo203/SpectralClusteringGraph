# Makefile for SpectralClusteringGraph with LIS integration
# Usage:
#   make all     -> build LIS (if needed), compile challenge2
#   make run     -> execute full pipeline and generate report
#   make clean   -> remove built files (keeps LIS)

CXX := g++
MPICC := mpicc
MPIRUN := mpirun

# Eigen include path
mkEigenInc ?= /usr/include/eigen3
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I$(mkEigenInc)

# LIS configuration (already in repo)
LIS_VERSION := 2.1.10
LIS_DIR := lis-$(LIS_VERSION)
mkLisInc := $(LIS_DIR)/include
mkLisLib := $(LIS_DIR)/lib
LIS_TEST_DIR := $(LIS_DIR)/test

# Directories
SRC_DIR := src
BUILD_DIR := build
DATA_DIR := data
MATRICES_DIR := matrices
REPORTS_DIR := reports

# C++ source and target
SRC := $(SRC_DIR)/challenge2.cpp
OBJ := $(BUILD_DIR)/challenge2.o
TARGET := $(BUILD_DIR)/challenge2

TASK10_SRC := $(SRC_DIR)/task10_11.cpp
TASK10_OBJ := $(BUILD_DIR)/task10_11.o
TASK10_TARGET := $(BUILD_DIR)/task10

# LIS executables
EIGEN1 := $(LIS_TEST_DIR)/etest1
TASK9 := $(LIS_TEST_DIR)/etest5

# Report file
REPORT := $(REPORTS_DIR)/challenge2_report.txt

.PHONY: all run clean help install-lis dirs

all: dirs install-lis $(TARGET) $(TASK10_TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(REPORTS_DIR) $(MATRICES_DIR)

# Build LIS library if not already built
install-lis: $(mkLisLib)/liblis.a

$(mkLisLib)/liblis.a:
	@if [ ! -f $(LIS_DIR)/include/Makefile.in ]; then \
		echo "==> Extracting fresh LIS source..."; \
		rm -rf $(LIS_DIR); \
		tar -xzf lis-$(LIS_VERSION).tar.gz; \
	fi
	@echo "==> Configuring and building LIS..."
	cd $(LIS_DIR) && ./configure --prefix=$(PWD)/$(LIS_DIR)
	$(MAKE) -C $(LIS_DIR)/src all
	@mkdir -p $(mkLisLib)
	@cp $(LIS_DIR)/src/.libs/liblis.a $(mkLisLib)/
	@echo "==> LIS library built successfully"

# Compile challenge2
$(TARGET): $(OBJ) | $(mkLisLib)/liblis.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJ): $(SRC) | dirs
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile task10
$(TASK10_TARGET): $(TASK10_OBJ) | $(mkLisLib)/liblis.a
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TASK10_OBJ): $(TASK10_SRC) | dirs
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile LIS test programs
$(EIGEN1) $(TASK9): $(mkLisLib)/liblis.a
	@echo "==> Compiling LIS test programs..."
	$(MAKE) -C $(LIS_TEST_DIR) etest1 etest5

# Run the complete pipeline
run: all $(EIGEN1) $(TASK9)
	@echo "==> Running complete analysis pipeline..."
	@echo "==> Executing challenge2..."
	@cd $(MATRICES_DIR) && ../$(TARGET)
	@echo "" >> $(REPORT)
	@echo "=====================================" >> $(REPORT)
	@echo "LIS EIGENVALUE COMPUTATIONS" >> $(REPORT)
	@echo "=====================================" >> $(REPORT)
	@echo "" >> $(REPORT)
	@echo "--- Task 7: Power Iteration (default shift) ---" >> $(REPORT)
	@cd $(MATRICES_DIR) && ../$(EIGEN1) Ls.mtx eigvec.txt hist.txt -e pi -etol 1.e-8 -emaxiter 5000 2>&1 | tee -a ../$(REPORT) || true
	@echo "" >> $(REPORT)
	@echo "--- Task 8: Power Iteration (shift=29.6) ---" >> $(REPORT)
	@cd $(MATRICES_DIR) && ../$(EIGEN1) Ls.mtx eigvec.txt hist.txt -e pi -etol 1.e-8 -emaxiter 5000 -shift 29.6 2>&1 | tee -a ../$(REPORT) || true
	@echo "" >> $(REPORT)
	@echo "--- Task 9: Subspace Iteration ---" >> $(REPORT)
	@cd $(MATRICES_DIR) && ../$(TASK9) Ls.mtx evalues9.txt evectors9.mtx residuals9.mtx iters9.mtx -e si -ss 2 -emaxiter 20000 -etol 1e-10 2>&1 | tee -a ../$(REPORT) || true
	@echo "" >> $(REPORT)
	@echo "Second smallest positive eigenvalue:" >> $(REPORT)
	@(grep -E '^2[[:space:]]+' $(MATRICES_DIR)/evalues9.txt | awk '{print $$2}' >> $(REPORT)) || echo "N/A" >> $(REPORT)
	@echo "Iterations count:" >> $(REPORT)
	@(grep -E '^2[[:space:]]+' $(MATRICES_DIR)/iters9.mtx | awk '{print $$2}' >> $(REPORT)) || echo "N/A" >> $(REPORT)
	@echo "" >> $(REPORT)
	@echo "==> Running task10 (Fiedler vector analysis)..."
	@cd $(MATRICES_DIR) && ../$(TASK10_TARGET) || echo "Warning: task10 failed" >> ../$(REPORT)
	@echo "=====================================" >> $(REPORT)
	@echo "Report complete" >> $(REPORT)
	@echo "=====================================" >> $(REPORT)
	@cat $(REPORT)
	@echo ""
	@echo "==> Analysis complete. Full report saved to $(REPORT)"

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LIS_TEST_DIR)/etest1 $(LIS_TEST_DIR)/etest5
	rm -f $(REPORT)
	rm -rf $(MATRICES_DIR)
	@echo "Build artifacts cleaned (LIS library preserved)"

help:
	@echo "Makefile for SpectralClusteringGraph with LIS"
	@echo ""
	@echo "Targets:"
	@echo "  make all              Build LIS (if needed), compile challenge2 and task10"
	@echo "  make run              Execute full pipeline with LIS tests"
	@echo "  make clean            Remove build artifacts (keep LIS)"
	@echo "  make mkEigenInc=DIR   Build with alternate Eigen path"
	@echo ""
	@echo "Directory structure:"
	@echo "  src/         - Source code"
	@echo "  build/       - Compiled binaries"
	@echo "  data/        - Input data"
	@echo "  matrices/    - Generated matrices"
	@echo "  reports/     - Output reports"