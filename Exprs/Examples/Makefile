CC=clang++
HOME_DIR := $(shell echo $$HOME)
LINK_FLAGS=-lnuma -std=c++20 -pthread 

STATIC_LINK_FLAGS=../Node/bin/libnode.a ../Stack/bin/libstack.a ../Queue/bin/libqueue.a ../BinarySearch/bin/libbinarysearch.a ../LinkdedList/bin/liblinkedlist.a
INC_DIRS= -I../include/  -I$(HOME_DIR)/NUMATyping/numaLib
UMF_INC_DIRS= 


EXE=./bin/DSExample
HIST_EXE=./bin/HistogramExample
OBJS=main.o TestSuite.o Histogram.o

UMF =
FLAGS = -fno-omit-frame-pointer
ifndef DEBUG
#  FLAGS += -DEBUG
else
	FLAGS += -DDEBUG
endif

ifeq ($(UMF), 1)
    LINK_FLAGS += -lhwloc -lnuma -lrt -ldl -ljemalloc  $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libumf.a $(HOME_DIR)/NUMATyping/unified-memory-framework/build/lib/libjemalloc_pool.a
	
    UMF_INC_DIRS+= -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/utils -I$(HOME_DIR)/NUMATyping/unified-memory-framework/include -I$(HOME_DIR)/NUMATyping/unified-memory-framework/examples/common -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/ravl -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/critnib -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/provider -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memspaces -I$(HOME_DIR)/NUMATyping/unified-memory-framework/src/memtargets -DUMF
endif

# $(EXE): $(OBJS)
# 	$(CC) $(OBJS) -o $(EXE) 
# -DPIN_INIT=1 
all: $(EXE) $(TESTEXE)

$(TESTEXE): $(TESTOBJS)
	$(CC) $(TESTOBJS) $(INC_DIRS) $(LINK_FLAGS) -o $(TESTEXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(EXE) 

main.o: 
	$(CC) -c -O3 -g -std=c++20  -pthread $(INC_DIRS) $(UMF_INC_DIRS) $(FLAGS) -D_NODE_HPP=1 -DPIN_INIT=1  main.cpp 
	

TestSuite.o: TestSuite.cpp
	$(CC) -c  -O3 -g -std=c++20  -pthread $(INC_DIRS) $(UMF_INC_DIRS) $(FLAGS)  TestSuite.cpp 

Histogram.o: Histogram.cpp
	$(CC) -c  -O3 -g -std=c++20  -pthread $(INC_DIRS) $(UMF_INC_DIRS) $(FLAGS)  Histogram.cpp


clean:
	rm *.o $(EXE)
