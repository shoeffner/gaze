cmake_minimum_required(VERSION 3.5)

# Project options
project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INSTALL_MESSAGE LAZY)


# Package options
if(NOT Boost_VERSION)
    cmake_minimum_required(VERSION 3.9.1)
    set(Boost_VERSION 1.65.0)
endif(NOT Boost_VERSION)
set(Boost_COMPONENTS program_options)

if(NOT OpenCV_VERSION)
    set(OpenCV_VERSION 3.3.0)
endif(NOT OpenCV_VERSION)
set(OpenCV_COMPONENTS core highgui imgcodecs imgproc videoio objdetect)

set(GTK_MODULE gtk+-3.0)


# Find packages
find_package(PkgConfig REQUIRED)
find_package(Boost ${Boost_VERSION} REQUIRED COMPONENTS ${Boost_COMPONENTS})
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED ${GTK_MODULE})


# Includes and libraries
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${GTK_INCLUDE_DIRS}
)
link_directories(
    ${Boost_LIBRARY_DIRS}
    ${OpenCV_LIBRARY_DIRS}
    ${GTK_LIBRARY_DIRS}
)


# Library
list(APPEND LIBRARY_SOURCE_FILES
  src/gaze.cpp
)

add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
if(${PROJECT_NAME}_DEPENDENCIES)
    add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
endif(${PROJECT_NAME}_DEPENDENCIES)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${GTK_LIBRARIES}
)


set(SIMPLE_VIEWER_TARGET gaze_viewer)
add_executable(${SIMPLE_VIEWER_TARGET} src/demo/simple_viewer.cpp)
add_dependencies(${SIMPLE_VIEWER_TARGET} ${PROJECT_NAME})
target_link_libraries(${SIMPLE_VIEWER_TARGET} ${PROJECT_NAME})
