cmake_minimum_required(VERSION 3.10.0)

# Project options
project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(MessageVariables)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_MESSAGE LAZY)

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

