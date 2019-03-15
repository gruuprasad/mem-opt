cmake_minimum_required(VERSION 3.4.3)
project(tas-opt)

#Script debug purpose: print to check cmake variable value
#message(STATUS "Value is: ${CMAKE_PREFIX_PATH}")

set(CMAKE_PREFIX_PATH "" CACHE STRING "Location to search for LLVM directory")

# C++, compiler related flags and build type
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fno-rtti")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -fno-rtti")

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release" FORCE)
endif()

message(STATUS "Using Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Using Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

if(DEFINED CMAKE_BUILD_TYPE)
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(LLVM_DIR "/home/gp/llvm_projects/llvm/install/lib/cmake/llvm")
# Find LLVM libraries
find_package(LLVM REQUIRED CONFIG)
if (LLVM_FOUND)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
else()
  message(FATAL_ERROR "LLVM libraries not found. Add location to CMAKE_PREFIX_PATH.")
endif()

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

include_directories(src/)
add_subdirectory(src/)
add_subdirectory(test/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_test.sh DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
