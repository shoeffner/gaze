cmake_minimum_required(VERSION 3.5)

# Project options
project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(MessageVariables)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_MESSAGE LAZY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library
add_subdirectory(${CMAKE_SOURCE_DIR}/src/gaze)

# Demo projects and options
if(BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/demo)
endif(BUILD_EXAMPLES)


# Build tags file if available
find_program(CTAGS NAMES ctags)
if(CTAGS)
  set(CTAGS_FILE tags)
  add_custom_target(${CTAGS_FILE}
                     COMMAND ${CTAGS} -R -f ${CTAGS_FILE}
                             ${CMAKE_SOURCE_DIR}/src
                             ${CMAKE_SOURCE_DIR}/include
                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/.git)
  add_dependencies(${PROJECT_NAME} ${CTAGS_FILE})
endif(CTAGS)
