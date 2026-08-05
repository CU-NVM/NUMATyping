# Compiler and basic settings
CC         := clang++
CXXFLAGS   := -O3 -g -std=c++20 -pthread -fno-omit-frame-pointer -D_NODE_HPP=1 -DPIN_INIT=1
LINK_FLAGS := -lnuma -g -O3 -std=c++20 -pthread

# This test only allocates on node 0, so no node-count macros are needed.
# ROOT_DIR: Use provided value, otherwise default to $HOME/NUMATyping
ROOT_DIR   ?= $(HOME)/NUMATyping

# Include directories
INC_DIRS := -Iinclude/  -I$(ROOT_DIR)/numaLib
UMF_INC_DIRS :=

# Executables.
#   allocator_test     - malloc|jemalloc|umf|numa micro-test (builds without UMF too)
#   verify_allocator   - regression test: fast-path speed + NUMA placement (UMF only)
#   throughput_compare - UMF vs numa_alloc_onnode speedup sweep      (UMF only)
EXE      := ./bin/allocator_test
UMF_EXES := ./bin/verify_allocator ./bin/throughput_compare

# Unified Memory Framework (optional)
# Usage: make UMF=1 JEMALLOC_ROOT=/path/to/jemalloc
ifeq ($(UMF), 1)
    LINK_FLAGS += -lhwloc -lnuma -lrt -ldl -ljemalloc \
        $(ROOT_DIR)/unified-memory-framework/build/lib/libumf.a \
        $(ROOT_DIR)/unified-memory-framework/build/lib/libjemalloc_pool.a

    # -DUMF gates the UMF code paths; node-count defines come from CXXFLAGS (line 3).
    UMF_INC_DIRS += -I$(ROOT_DIR)/unified-memory-framework/src/utils \
        -I$(ROOT_DIR)/unified-memory-framework/include \
        -I$(ROOT_DIR)/unified-memory-framework/examples/common \
        -I$(ROOT_DIR)/unified-memory-framework/src \
        -I$(ROOT_DIR)/unified-memory-framework/src/ravl \
        -I$(ROOT_DIR)/unified-memory-framework/src/critnib \
        -I$(ROOT_DIR)/unified-memory-framework/src/provider \
        -I$(ROOT_DIR)/unified-memory-framework/src/memspaces \
        -I$(ROOT_DIR)/unified-memory-framework/src/memtargets \
        -DUMF
    # Only add Jemalloc-specific flags if JEMALLOC_ROOT is defined
    ifdef JEMALLOC_ROOT
        UMF_INC_DIRS += -I$(JEMALLOC_ROOT)/include
        LINK_FLAGS   += -L$(JEMALLOC_ROOT)/lib -Wl,-rpath,$(JEMALLOC_ROOT)/lib
    endif
endif

# Enable DEBUG if defined
ifdef DEBUG
    CXXFLAGS += -DDEBUG
endif

# Default target: always build allocator_test; add the UMF-only tests under UMF=1.
all: $(EXE)
ifeq ($(UMF), 1)
all: $(UMF_EXES)
endif

# allocator_test links the shared main.o
./bin/allocator_test: src/main.o
	@mkdir -p bin
	$(CC) $< $(LINK_FLAGS) -o $@

# The UMF-only tests are each a single self-contained .cpp.
./bin/verify_allocator: src/verify_allocator.cpp src/alloc_test_common.hpp
	@mkdir -p bin
	$(CC) $(CXXFLAGS) $(INC_DIRS) $(UMF_INC_DIRS) src/verify_allocator.cpp $(LINK_FLAGS) -o $@

./bin/throughput_compare: src/throughput_compare.cpp src/alloc_test_common.hpp
	@mkdir -p bin
	$(CC) $(CXXFLAGS) $(INC_DIRS) $(UMF_INC_DIRS) src/throughput_compare.cpp $(LINK_FLAGS) -o $@

# Generic rule for compiling .cpp to .o (used by main.cpp)
%.o: %.cpp
	$(CC) -c $(CXXFLAGS) $(INC_DIRS) $(UMF_INC_DIRS) $< -o $@

# Clean build artifacts
clean:
	rm -f src/*.o $(EXE) $(UMF_EXES)

