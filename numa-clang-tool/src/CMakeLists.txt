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

# --- 2. Include Configuration ---
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(SYSTEM ${CLANG_INCLUDE_DIRS})
include_directories(SYSTEM ${CLANG_BUILTIN_INCLUDE_DIR})

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
    ${LLVM_SHARED_LIB} 
    ${LLVM_LDFLAGS_STRIPPED} 
    jsoncpp_lib
)

# Filesystem compatibility fix for older GCC/RHEL systems
find_library(FILESYSTEM_LIB NAMES stdc++fs)
if(FILESYSTEM_LIB)
    target_link_libraries(clang-tool PRIVATE ${FILESYSTEM_LIB})
endif()
