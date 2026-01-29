cmake_minimum_required(VERSION 2.8)
project(clang-tool)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/)

# Allow user to specify clang version manually (e.g., -DCLANG_VER=21)
set(CLANG_VER "" CACHE STRING "Clang version number (e.g.20, 21)" FORCE)

if (NOT CLANG_VER)
    # Auto-detect clang version if not provided
    execute_process(
        COMMAND bash -c "clang++ --version | grep -Eo 'clang version [0-9]+' | awk '{print $3}' | head -n1"
        OUTPUT_VARIABLE CLANG_VER
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (CLANG_VER)
        message(STATUS "Auto-detected Clang version: ${CLANG_VER}")
    else()
        message(FATAL_ERROR "Failed to detect clang version automatically. Please pass -DCLANG_VER=<version> manually.")
    endif()
else()
    message(STATUS "Using user-provided Clang version: ${CLANG_VER}")
endif()

# Build include path using the chosen version
set(CLANG_BUILTIN_INCLUDE_DIR "/usr/local/lib/clang/${CLANG_VER}/include")
message(STATUS "Using Clang builtin include path: ${CLANG_BUILTIN_INCLUDE_DIR}")

# Write the path for downstream tools
file(WRITE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/builtInInclude.path" "${CLANG_BUILTIN_INCLUDE_DIR}\n")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
add_subdirectory(src)