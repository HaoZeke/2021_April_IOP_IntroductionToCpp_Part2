cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

project(
  PenningTrapSimulationII
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# --- Helpers
include(cmake/CPM.cmake)

# -- External
CPMAddPackage(
  NAME pybind11
  GITHUB_REPOSITORY pybind/pybind11
  GIT_TAG v2.6.1
)

# --- Variables

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ---- CMake Output
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "CXX compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "Python: " ${Python_EXECUTABLE})

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  message(STATUS "CXX compiler flags:" ${CMAKE_CXX_FLAGS}
                 ${CMAKE_CXX_FLAGS_RELEASE} ${COMPILE_OPTIONS})
elseif(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  message(STATUS "CXX compiler flags:" ${CMAKE_CXX_FLAGS}
                 ${CMAKE_CXX_FLAGS_DEBUG} ${COMPILE_OPTIONS})
endif()
 
set (CMAKE_CXX_STANDARD 11)

# --- Library
set(headers
  "include/data_types/iop_vec.hpp"
  "include/data_types/iop_particle.hpp"
  )
set(sources "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")


# Build
add_executable(simulation.exe ${sources})
target_include_directories(
  simulation.exe PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Build order
add_dependencies(simulation.exe ioplib)
# Libraries
target_link_libraries(simulation.exe ioplib pybind11::embed)
 
# Dependencies
add_library(
  ioplib SHARED
  "libsrc/phys_procs.cpp"
  )
target_include_directories(
  ioplib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Catch2
include(CTest)
add_subdirectory(tests)
enable_testing()
