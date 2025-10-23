CXX := g++
CXXFLAGS := -std=c++17 -O3 -Wall
LDFLAGS := -pthread -lnuma -ljemalloc

VPATH := .:../zipf_base/src:../../numaLib

RUNNER_SRCS := main.cpp ycsb_benchmark.cpp zipfian_generator.cpp ycsbutils.cpp

RUNNER_OBJS := $(RUNNER_SRCS:.cpp=.o)
RUNNER_TARGET := runner

all: $(RUNNER_TARGET)

$(RUNNER_TARGET): $(RUNNER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(RUNNER_TARGET) $(RUNNER_OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I../include -I../zipf_base/src -I../../numaLib -c $< -o $@

clean:
	rm -f $(RUNNER_OBJS) $(RUNNER_TARGET)