cmake_minimum_required(VERSION 3.5)

#################
# PROJECT OPTIONS
#################

project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(MessageVariables)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INSTALL_MESSAGE LAZY)


#################
# LIBRARY OPTIONS
#################

if(NOT OpenCV_VERSION)
    set(OpenCV_VERSION 3.3.0)
endif(NOT OpenCV_VERSION)
set(OpenCV_COMPONENTS core highgui imgcodecs imgproc videoio objdetect)

# Find packages
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})

# Includes and libraries
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)
link_directories(
    ${OpenCV_LIBRARY_DIRS}
)

# Library
list(APPEND LIBRARY_SOURCE_FILES
  src/gaze.cpp
)

add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
if(${OpenCV_TARGET})
    add_dependencies(${PROJECT_NAME} ${OpenCV_TARGET})
endif(${OpenCV_TARGET})

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBRARIES}
)


###########################
# DEMO PROJECTS AND OPTIONS
###########################

if(BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/demo)
endif(BUILD_EXAMPLES)
