cmake_minimum_required(VERSION 3.10.0)

# Project options
set(VERSION 0.1)
project(gaze VERSION ${VERSION} LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(MessageVariables)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_MESSAGE LAZY)

# Caffe/GazeCapture(iTracker) support
option(WITH_CAFFE "Build with caffe support." OFF)
if(WITH_CAFFE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/iTracker)
endif(WITH_CAFFE)

# Library
add_subdirectory(${CMAKE_SOURCE_DIR}/src/gaze)

# Tests
option(BUILD_TESTS "Build tests" ON)
if(${BUILD_TESTS})
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif(${BUILD_TESTS})

# Demo projects and options
option(BUILD_EXAMPLES "Build examples." OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/demo)
endif(BUILD_EXAMPLES)

option(BUILD_DOCS "Build docs." ON)
if(BUILD_DOCS)
    include(DocsTarget)
endif(BUILD_DOCS)

option(BUILD_CTAGS "Build ctags." OFF)
if(BUILD_CTAGS)
    include(CTagsTarget)
endif(BUILD_CTAGS)

