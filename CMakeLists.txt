cmake_minimum_required(VERSION 3.5)

# Project options
project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INSTALL_MESSAGE LAZY)

message(STATUS ${CMAKE_INSTALL_PREFIX})

# Package options
set(Boost_VERSION 1.65.0)
set(Boost_COMPONENTS program_options)

set(OpenCV_VERSION 3.3.0)
set(OpenCV_COMPONENTS core highgui imgcodecs imgproc videoio objdetect)


# Find packages
find_package(Boost ${Boost_VERSION} REQUIRED COMPONENTS ${Boost_COMPONENTS})
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})


# Includes and libraries
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)
link_directories(
    ${Boost_LIBRARY_DIRS}
    ${OpenCV_LIBRARY_DIRS}
)


# Executable and dependencies
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
if(${PROJECT_NAME}_DEPENDENCIES)
    add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
endif(${PROJECT_NAME}_DEPENDENCIES)


# Linking
target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBRARIES}
)


# Installation
if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
endif(NOT CMAKE_INSTALL_PREFIX)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY
    ${OpenCV_DIR}/haarcascades
    ${OpenCV_DIR}/lbpcascades
    DESTINATION ${CMAKE_INSTALL_PREFIX}/assets
)
