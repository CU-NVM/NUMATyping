# Copyright (C) 2022-2024 Intel Corporation
cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

# --- 0. Terminal Help Interface ---
set(HELP_MSG "
UMF Build Options:
  -DHELP=ON                     Print this help message and exit.
  -DJEMALLOC_ROOT=<path>        Manually specify the Jemalloc installation prefix.
  -DHWLOC_ROOT=<path>           Manually specify the HWLOC installation prefix.
")
if(HELP)
    message("${HELP_MSG}")
    return()
endif()

# Set root directory correctly
set(UMF_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UMF_BUILD_LIBUMF_POOL_JEMALLOC ON)
list(APPEND CMAKE_MODULE_PATH "${UMF_CMAKE_SOURCE_DIR}/cmake")

# Load PkgConfig BEFORE it is used by any modules
find_package(PkgConfig REQUIRED)

include(${UMF_CMAKE_SOURCE_DIR}/cmake/helpers.cmake)
set_version_variables()
message(STATUS "UMF version: ${UMF_VERSION}")

project(umf VERSION ${UMF_CMAKE_VERSION} LANGUAGES C)

# Force all libraries (.a, .so) into a single lib folder inside the build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CTest)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ... [Keep your existing option() and logic blocks here] ...

# --- 1. HWLOC Discovery Refactor ---
if(NOT UMF_DISABLE_HWLOC)
    if(HWLOC_ROOT)
        list(APPEND CMAKE_PREFIX_PATH "${HWLOC_ROOT}")
    endif()
    pkg_check_modules(LIBHWLOC hwloc>=2.3.0)
    if(NOT LIBHWLOC_FOUND)
        find_package(LIBHWLOC 2.3.0 REQUIRED hwloc)
    endif()
endif()

# --- 2. JEMALLOC Discovery Refactor ---
if(UMF_BUILD_LIBUMF_POOL_JEMALLOC)
    if(JEMALLOC_ROOT)
        message(STATUS "Using user-provided JEMALLOC_ROOT: ${JEMALLOC_ROOT}")
        set(JEMALLOC_INCLUDE_DIRS "${JEMALLOC_ROOT}/include")
        set(JEMALLOC_LIBRARIES "${JEMALLOC_ROOT}/lib/libjemalloc.so")
        set(JEMALLOC_FOUND TRUE)
    else()
        pkg_check_modules(JEMALLOC jemalloc)
    endif()
    
    if(JEMALLOC_FOUND)
        set(UMF_POOL_JEMALLOC_ENABLED TRUE)
    else()
        message(FATAL_ERROR "Jemalloc not found. Pass -DJEMALLOC_ROOT=<path>")
    endif()
endif()

# Important: This is the ONLY place add_subdirectory(src) should exist
add_subdirectory(src)

# ... [Keep the rest of your install logic] ...
