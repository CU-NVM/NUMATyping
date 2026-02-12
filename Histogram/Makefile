CC=clang++
CXX=clang++
HOME_DIR := $(shell echo $$HOME)

# Turn sanitizers on everywhere (compile + link)
SANITIZE = -fsanitize=address,undefined -fno-omit-frame-pointer

LINK_FLAGS = -std=c++20 -pthread  -lnuma
INC_DIRS   = -Iinclude/ -Iutil/ -I$(HOME_DIR)/NUMATyping/numaLib
UMF_INC_DIRS =
EXE=./bin/histogram
OBJS=main.o Histogram.o

UMF =
FLAGS = -fno-omit-frame-pointer
ifdef DEBUG
  FLAGS += -DDEBUG -DNUMA_MACHINE
endif

ifeq ($(UMF), 1)
  LINK_FLAGS += -lhwloc -lrt -ldl -ljemalloc \
    $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libumf.a \
    $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libjemalloc_pool.a -DUMF
  UMF_INC_DIRS += -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/utils \
  -I$(HOME_DIR)/NUMATyping/unified-memory-framework/include -I$(HOME_DIR)/NUMATyping/unified-memory-framework/examples/common \
  -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/ravl \
  -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/critnib -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/provider \
  -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memspaces -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memtargets -DUMF -g3
endif

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LINK_FLAGS) -o $(EXE)

CXXFLAGS_COMMON = -O0 -g3 -std=c++20 -pthread $(INC_DIRS) $(UMF_INC_DIRS) $(FLAGS)
#-fno-strict-aliasing \
                  -fno-vectorize -fno-unroll-loops $(SANITIZE)

main.o: src/main.cpp
	$(CXX) -c $(CXXFLAGS_COMMON) -D_NODE_HPP=1 -DPIN_INIT=1 src/main.cpp

Histogram.o: src/Histogram.cpp
	$(CXX) -c $(CXXFLAGS_COMMON) src/Histogram.cpp

clean:
	rm -f *.o $(EXE)