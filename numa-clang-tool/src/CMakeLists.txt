cmake_minimum_required(VERSION 3.12)

# Find dependencies (uses modernized FindLLVM.cmake to avoid warnings)
find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)
find_package(jsoncpp REQUIRED)

# --- 1. Dynamic Library Discovery Macro ---
# Order: Variable Name, Search Directory, File Pattern
macro(find_shared_lib_dynamic NAME_VAR HINT_DIR PATTERN)
    if(NOT ${NAME_VAR})
        file(GLOB CANDIDATES "${HINT_DIR}/${PATTERN}")
        if(CANDIDATES)
            list(GET CANDIDATES 0 ${NAME_VAR})
            set(${NAME_VAR} "${${NAME_VAR}}" CACHE PATH "Path to ${PATTERN}")
            message(STATUS "Found Shared Library ${PATTERN}: ${${NAME_VAR}}")
        else()
            message(FATAL_ERROR 
                " Could not automatically find ${PATTERN} in ${HINT_DIR}.\n"
                " Please specify manually: cmake -D${NAME_VAR}=/path/to/file.so ..\n"
                " Hint: On Perlmutter, ensure PrgEnv-llvm and llvm modules are loaded."
            )
        endif()
    else()
        message(STATUS "Using user-provided ${NAME_VAR}: ${${NAME_VAR}}")
    endif()
endmacro()

# Discover shared libraries (HINT_DIR comes from FindLLVM.cmake)
find_shared_lib_dynamic(LLVM_SHARED_LIB "${LLVM_LIB_DIR}" "libLLVM*.so")
find_shared_lib_dynamic(CLANG_CPP_SHARED_LIB "${LLVM_LIB_DIR}" "libclang-cpp*.so")

# --- 1b. Should libLLVM.so be linked separately? ---
# LLVM installations come in two shapes:
#
#   (a) libclang-cpp.so has a DT_NEEDED on libLLVM.so, i.e. one shared copy of
#       LLVM that both libraries use.  Linking both is correct and required.
#       (stormbreaker's hand-built /usr/local trunk LLVM is this shape.)
#
#   (b) libclang-cpp.so statically embeds its own complete copy of LLVM and has
#       no DT_NEEDED on libLLVM.so.  Linking both then loads TWO full copies of
#       LLVM's globals into one process.  (NERSC PrgEnv-llvm/21.1.4 is this
#       shape: both .so files define llvm::cl and llvm::TensorSpec symbols.)
#
# In shape (b) the duplicate globals interpose at load time, then BOTH libraries
# run their own static destructors at exit and the second frees storage owned by
# the first:
#
#       munmap_chunk(): invalid pointer
#       ... ~vector<llvm::TensorSpec>() in libLLVM.so -> __cxa_finalize -> _dl_fini
#
# The passes finish before this happens, so it looks like a harmless teardown
# crash -- but it still aborts the process, and scripts/numafy.py invokes the
# tool with check=True, so the whole transform fails.
#
# Detect the shape instead of hardcoding either one, so the same tree builds on
# both machines.  Override with -DLINK_LLVM_DYLIB=ON/OFF.
if(NOT DEFINED LINK_LLVM_DYLIB)
    find_program(READELF_EXECUTABLE NAMES readelf llvm-readelf)
    if(READELF_EXECUTABLE)
        execute_process(
            COMMAND ${READELF_EXECUTABLE} -d "${CLANG_CPP_SHARED_LIB}"
            OUTPUT_VARIABLE CLANG_CPP_DYNAMIC
            ERROR_QUIET)
        if(CLANG_CPP_DYNAMIC MATCHES "NEEDED[^\n]*libLLVM")
            set(LINK_LLVM_DYLIB ON)
            message(STATUS "libclang-cpp.so depends on libLLVM.so -- linking both")
        else()
            set(LINK_LLVM_DYLIB OFF)
            message(STATUS "libclang-cpp.so embeds its own LLVM -- NOT linking libLLVM.so "
                           "separately (avoids duplicate LLVM globals / exit-time abort)")
        endif()
    else()
        # Cannot inspect: keep the historical behaviour rather than guess.
        set(LINK_LLVM_DYLIB ON)
        message(WARNING "readelf not found; linking libLLVM.so as well. If the tool aborts "
                        "with 'munmap_chunk(): invalid pointer' at exit, configure with "
                        "-DLINK_LLVM_DYLIB=OFF.")
    endif()
endif()

if(LINK_LLVM_DYLIB)
    set(LLVM_DYLIB_TO_LINK "${LLVM_SHARED_LIB}")
else()
    set(LLVM_DYLIB_TO_LINK "")
endif()

# --- 2. Include Configuration ---
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(SYSTEM ${CLANG_INCLUDE_DIRS})
#include_directories(SYSTEM ${CLANG_BUILTIN_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -O3 -fno-rtti -fno-exceptions -funwind-tables -pthread")

# --- 3. Source Definitions ---
set(CLANG_TOOL_SRC
    main.cc
    actions/recurseFrontendAction.cc
    actions/castFrontendAction.cc
    consumer/recurseConsumer.cc
    consumer/castConsumer.cc
    utils/utils.cc
    numafy/new_allocs.cc
    casting/reinterprete_cast.cc
    transformer/transformer.cc
    transformer/RecursiveNumaTyper.cc
    transformer/CastNumaAlloc.cc
)

# --- 4. Build and Link ---
add_executable(clang-tool ${CLANG_TOOL_SRC})

# Strip potential whitespace from detected LDFLAGS
string(STRIP "${LLVM_LDFLAGS}" LLVM_LDFLAGS_STRIPPED)

target_link_libraries(clang-tool
    PRIVATE
    ${CLANG_CPP_SHARED_LIB}
    ${LLVM_DYLIB_TO_LINK}
    ${LLVM_LDFLAGS_STRIPPED}
    jsoncpp_lib
)

# Filesystem compatibility fix for older GCC/RHEL systems
find_library(FILESYSTEM_LIB NAMES stdc++fs)
if(FILESYSTEM_LIB)
    target_link_libraries(clang-tool PRIVATE ${FILESYSTEM_LIB})
endif()
