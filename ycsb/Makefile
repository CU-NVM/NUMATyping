# Compiler and basic settings
CC         := clang++
CXXFLAGS   := -O3 -g -std=c++20 -pthread -fno-omit-frame-pointer -D_NODE_HPP=1 -DPIN_INIT=1
LINK_FLAGS := -lnuma -g -O3 -std=c++20 -pthread
HOME_DIR   := $(shell echo $$HOME)

# Include directories
INC_DIRS := -Iinclude/ -Izipf_base/src -I$(HOME_DIR)/NUMATyping/numaLib
UMF_INC_DIRS :=

# Executable and object files
EXE  := ./bin/ycsb
OBJS := src/main.o src/ycsb_benchmark.o zipf_base/src/ycsbutils.o zipf_base/src/zipfian_generator.o

# Unified Memory Framework (optional)
UMF :=
ifeq ($(UMF), 1)
    LINK_FLAGS += -lhwloc -lnuma -lrt -ldl -ljemalloc \
        $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libumf.a \
        $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libjemalloc_pool.a

    UMF_INC_DIRS += -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/utils \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/include \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/examples/common \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/ravl \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/critnib \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/provider \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memspaces \
        -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memtargets \
        -DUMF
endif

# Enable DEBUG if defined
ifdef DEBUG
    CXXFLAGS += -DDEBUG
endif

# Default target
all: $(EXE)

# Linking
$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(EXE)

# Generic rule for compiling .cpp to .o
%.o: %.cpp
	$(CC) -c $(CXXFLAGS) $(INC_DIRS) $(UMF_INC_DIRS) $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(EXE)
