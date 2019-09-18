cmake_minimum_required(VERSION 3.4.3)

set(CMAKE_INSTALL_PREFIX "/usr/")
project(tas-opt)

#Script debug purpose: print to check cmake variable value
#message(STATUS "Value is: ${CMAKE_PREFIX_PATH}")

  set(CMAKE_PREFIX_PATH "/home/gp/llvm_projects//install" CACHE STRING "Location to search for LLVM directory")

# C++, compiler related flags and build type
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fno-rtti -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -fno-rtti -fPIC")

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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

find_package(glog 0.4.0 REQUIRED)

set(LLVM_DIR "/home/gp/llvm_projects/install/lib/cmake/llvm")
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

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found, `doc` target to generate html documentation, viewdoc to open in browser")
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation for tas-opt" VERBATIM
    )
  add_custom_target(viewdoc
    xdg-open ${CMAKE_BINARY_DIR}/doc/html/index.html
    COMMENT "Opening documentation in browser" VERBATIM
    )
endif(DOXYGEN_FOUND)

include_directories(src)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)

# Testing related tools
if(NOT EXISTS ${LLVM_ROOT}/bin/FileCheck)
    message(FATAL_ERROR "LLVM wasn't configured with -DLLVM_INSTALL_UTILS, cannot use FileCheck")
endif()

# Set lit configurations
#configure_file("${CMAKE_SOURCE_DIR}/test/litTests/lit.site.cfg.in" "${CMAKE_BINARY_DIR}/test/litTests/lit.site.cfg")

#add_custom_target(check COMMAND lit ${CMAKE_BINARY_DIR}/test/litTests)
#add_custom_target(check COMMAND ctest)
