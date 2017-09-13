cmake_minimum_required(VERSION 3.5)

#################
# PROJECT OPTIONS
#################

project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INSTALL_MESSAGE LAZY)


#################
# LIBRARY OPTIONS
#################

if(NOT Boost_VERSION)
    cmake_minimum_required(VERSION 3.9.1)
    set(Boost_VERSION 1.65.0)
endif(NOT Boost_VERSION)
set(Boost_COMPONENTS program_options)

if(NOT OpenCV_VERSION)
    set(OpenCV_VERSION 3.3.0)
endif(NOT OpenCV_VERSION)
set(OpenCV_COMPONENTS core highgui imgcodecs imgproc videoio objdetect)

# Find packages
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})

# Packages only needed for examples
if(BUILD_EXAMPLES)
    find_package(Boost ${Boost_VERSION} COMPONENTS ${Boost_COMPONENTS})
    find_package(PkgConfig)
    pkg_check_modules(GTK gtk+-3.0)
endif(BUILD_EXAMPLES)

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
)


###########################
# DEMO PROJECTS AND OPTIONS
###########################

if(BUILD_EXAMPLES)

# Debug viewer: Just show input stream
if(Boost_FOUND)
    set(SIMPLE_VIEWER_TARGET gaze_debug_viewer)
    add_executable(${SIMPLE_VIEWER_TARGET} src/demo/gaze_debug_viewer.cpp)
    target_link_libraries(${SIMPLE_VIEWER_TARGET} ${PROJECT_NAME})
else(Boost_FOUND)
    message(WARNING "Can not build gaze_debug_viewer example, did not find Boost.")
endif(Boost_FOUND)


# Where People Look, Judd et al. 2009

if(GTK_FOUND)
    set(WPL_TARGET where_people_look)
    add_executable(${WPL_TARGET} src/demo/where_people_look.cpp)
    target_link_libraries(${WPL_TARGET} ${PROJECT_NAME} ${GTK_LIBRARIES})
else(GTK_FOUND)
    message(WARNING "Can not build where_people_look example, did not find GTK 3.")
endif(GTK_FOUND)

endif(BUILD_EXAMPLES)
