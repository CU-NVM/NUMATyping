# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kidus/NUMATyping/numa-clang-tool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kidus/NUMATyping/numa-clang-tool/build

# Include any dependencies generated for this target.
include src/CMakeFiles/clang-tool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/clang-tool.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/clang-tool.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/clang-tool.dir/flags.make

src/CMakeFiles/clang-tool.dir/main.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/main.cc.o: ../src/main.cc
src/CMakeFiles/clang-tool.dir/main.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/clang-tool.dir/main.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/main.cc.o -MF CMakeFiles/clang-tool.dir/main.cc.o.d -o CMakeFiles/clang-tool.dir/main.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/main.cc

src/CMakeFiles/clang-tool.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/main.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/main.cc > CMakeFiles/clang-tool.dir/main.cc.i

src/CMakeFiles/clang-tool.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/main.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/main.cc -o CMakeFiles/clang-tool.dir/main.cc.s

src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o: ../src/actions/frontendaction.cc
src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o -MF CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o.d -o CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/actions/frontendaction.cc

src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/actions/frontendaction.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/actions/frontendaction.cc > CMakeFiles/clang-tool.dir/actions/frontendaction.cc.i

src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/actions/frontendaction.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/actions/frontendaction.cc -o CMakeFiles/clang-tool.dir/actions/frontendaction.cc.s

src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o: ../src/actions/cast_frontendaction.cc
src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o -MF CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o.d -o CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/actions/cast_frontendaction.cc

src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/actions/cast_frontendaction.cc > CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.i

src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/actions/cast_frontendaction.cc -o CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.s

src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o: ../src/consumer/consumer.cc
src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o -MF CMakeFiles/clang-tool.dir/consumer/consumer.cc.o.d -o CMakeFiles/clang-tool.dir/consumer/consumer.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/consumer/consumer.cc

src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/consumer/consumer.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/consumer/consumer.cc > CMakeFiles/clang-tool.dir/consumer/consumer.cc.i

src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/consumer/consumer.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/consumer/consumer.cc -o CMakeFiles/clang-tool.dir/consumer/consumer.cc.s

src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o: ../src/consumer/cast_consumer.cc
src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o -MF CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o.d -o CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/consumer/cast_consumer.cc

src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/consumer/cast_consumer.cc > CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.i

src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/consumer/cast_consumer.cc -o CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.s

src/CMakeFiles/clang-tool.dir/utils/utils.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/utils/utils.cc.o: ../src/utils/utils.cc
src/CMakeFiles/clang-tool.dir/utils/utils.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/clang-tool.dir/utils/utils.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/utils/utils.cc.o -MF CMakeFiles/clang-tool.dir/utils/utils.cc.o.d -o CMakeFiles/clang-tool.dir/utils/utils.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/utils/utils.cc

src/CMakeFiles/clang-tool.dir/utils/utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/utils/utils.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/utils/utils.cc > CMakeFiles/clang-tool.dir/utils/utils.cc.i

src/CMakeFiles/clang-tool.dir/utils/utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/utils/utils.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/utils/utils.cc -o CMakeFiles/clang-tool.dir/utils/utils.cc.s

src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o: ../src/transformer/transformer.cc
src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o -MF CMakeFiles/clang-tool.dir/transformer/transformer.cc.o.d -o CMakeFiles/clang-tool.dir/transformer/transformer.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/transformer/transformer.cc

src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/transformer/transformer.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/transformer/transformer.cc > CMakeFiles/clang-tool.dir/transformer/transformer.cc.i

src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/transformer/transformer.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/transformer/transformer.cc -o CMakeFiles/clang-tool.dir/transformer/transformer.cc.s

src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o: ../src/transformer/RecursiveNumaTyper.cc
src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o -MF CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o.d -o CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/transformer/RecursiveNumaTyper.cc

src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/transformer/RecursiveNumaTyper.cc > CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.i

src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/transformer/RecursiveNumaTyper.cc -o CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.s

src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o: src/CMakeFiles/clang-tool.dir/flags.make
src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o: ../src/transformer/NumaTargetNumaPointer.cc
src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o: src/CMakeFiles/clang-tool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o -MF CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o.d -o CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o -c /home/kidus/NUMATyping/numa-clang-tool/src/transformer/NumaTargetNumaPointer.cc

src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.i"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kidus/NUMATyping/numa-clang-tool/src/transformer/NumaTargetNumaPointer.cc > CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.i

src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.s"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kidus/NUMATyping/numa-clang-tool/src/transformer/NumaTargetNumaPointer.cc -o CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.s

# Object files for target clang-tool
clang__tool_OBJECTS = \
"CMakeFiles/clang-tool.dir/main.cc.o" \
"CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o" \
"CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o" \
"CMakeFiles/clang-tool.dir/consumer/consumer.cc.o" \
"CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o" \
"CMakeFiles/clang-tool.dir/utils/utils.cc.o" \
"CMakeFiles/clang-tool.dir/transformer/transformer.cc.o" \
"CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o" \
"CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o"

# External object files for target clang-tool
clang__tool_EXTERNAL_OBJECTS =

bin/clang-tool: src/CMakeFiles/clang-tool.dir/main.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/actions/frontendaction.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/actions/cast_frontendaction.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/consumer/consumer.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/consumer/cast_consumer.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/utils/utils.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/transformer/transformer.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/transformer/RecursiveNumaTyper.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/transformer/NumaTargetNumaPointer.cc.o
bin/clang-tool: src/CMakeFiles/clang-tool.dir/build.make
bin/clang-tool: /usr/local/lib/libclangFrontend.a
bin/clang-tool: /usr/local/lib/libclangDriver.a
bin/clang-tool: /usr/local/lib/libclangCodeGen.a
bin/clang-tool: /usr/local/lib/libclangSema.a
bin/clang-tool: /usr/local/lib/libclangAnalysis.a
bin/clang-tool: /usr/local/lib/libclangRewrite.a
bin/clang-tool: /usr/local/lib/libclangAST.a
bin/clang-tool: /usr/local/lib/libclangParse.a
bin/clang-tool: /usr/local/lib/libclangLex.a
bin/clang-tool: /usr/local/lib/libclangBasic.a
bin/clang-tool: /usr/local/lib/libclangARCMigrate.a
bin/clang-tool: /usr/local/lib/libclangEdit.a
bin/clang-tool: /usr/local/lib/libclangFrontendTool.a
bin/clang-tool: /usr/local/lib/libclangRewrite.a
bin/clang-tool: /usr/local/lib/libclangSerialization.a
bin/clang-tool: /usr/local/lib/libclangTooling.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerCheckers.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerCore.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerFrontend.a
bin/clang-tool: /usr/local/lib/libclangSema.a
bin/clang-tool: /usr/local/lib/libclangRewriteFrontend.a
bin/clang-tool: /usr/local/lib/libclangASTMatchers.a
bin/clang-tool: /usr/local/lib/libclangToolingCore.a
bin/clang-tool: /usr/local/lib/libclang-cpp.so
bin/clang-tool: /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.9.5
bin/clang-tool: /usr/local/lib/libclangAnalysis.a
bin/clang-tool: /usr/local/lib/libclangRewrite.a
bin/clang-tool: /usr/local/lib/libclangAST.a
bin/clang-tool: /usr/local/lib/libclangParse.a
bin/clang-tool: /usr/local/lib/libclangLex.a
bin/clang-tool: /usr/local/lib/libclangBasic.a
bin/clang-tool: /usr/local/lib/libclangARCMigrate.a
bin/clang-tool: /usr/local/lib/libclangEdit.a
bin/clang-tool: /usr/local/lib/libclangFrontendTool.a
bin/clang-tool: /usr/local/lib/libclangSerialization.a
bin/clang-tool: /usr/local/lib/libclangTooling.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerCheckers.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerCore.a
bin/clang-tool: /usr/local/lib/libclangStaticAnalyzerFrontend.a
bin/clang-tool: /usr/local/lib/libclangRewriteFrontend.a
bin/clang-tool: /usr/local/lib/libclangASTMatchers.a
bin/clang-tool: /usr/local/lib/libclangToolingCore.a
bin/clang-tool: /usr/local/lib/libclang-cpp.so
bin/clang-tool: src/CMakeFiles/clang-tool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kidus/NUMATyping/numa-clang-tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable ../bin/clang-tool"
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clang-tool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/clang-tool.dir/build: bin/clang-tool
.PHONY : src/CMakeFiles/clang-tool.dir/build

src/CMakeFiles/clang-tool.dir/clean:
	cd /home/kidus/NUMATyping/numa-clang-tool/build/src && $(CMAKE_COMMAND) -P CMakeFiles/clang-tool.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/clang-tool.dir/clean

src/CMakeFiles/clang-tool.dir/depend:
	cd /home/kidus/NUMATyping/numa-clang-tool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kidus/NUMATyping/numa-clang-tool /home/kidus/NUMATyping/numa-clang-tool/src /home/kidus/NUMATyping/numa-clang-tool/build /home/kidus/NUMATyping/numa-clang-tool/build/src /home/kidus/NUMATyping/numa-clang-tool/build/src/CMakeFiles/clang-tool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/clang-tool.dir/depend

