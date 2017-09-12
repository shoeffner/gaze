cmake_minimum_required(VERSION 3.5)

# Project options
project(gaze VERSION 0.1 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 98)
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


# Executable and dependencies
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
if(${PROJECT_NAME}_DEPENDENCIES)
    add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
endif(${PROJECT_NAME}_DEPENDENCIES)


# Linking
target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${GTK_LIBRARIES}
)


# Installation
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
endif("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})


# Install cascade files on systems where they are not in /usr/share/OpenCV
# but /usr/share/opencv
list(APPEND CASCADE_DIRS
    "${OpenCV_DIR}/haarcascades"
    "${OpenCV_DIR}/lbpcascades"
)
foreach(CASCADE_DIR ${CASCADE_DIRS})
    if(NOT EXISTS ${CASCADE_DIR})
        list(REMOVE_ITEM CASCADE_DIRS ${CASCADE_DIR})
        string(TOLOWER ${CASCADE_DIR} CASCADE_DIR)
        if(EXISTS ${CASCADE_DIR})
            list(APPEND CASCADE_DIRS ${CASCADE_DIR})
        endif(EXISTS ${CASCADE_DIR})
    endif(NOT EXISTS ${CASCADE_DIR})
endforeach(CASCADE_DIR)
list(LENGTH CASCADE_DIRS CASCADE_DIRS_LENGTH)
if(NOT ${CASCADE_DIRS_LENGTH} EQUAL 2)
    message(ERROR "Can not find all cascade directories.")
endif(NOT ${CASCADE_DIRS_LENGTH} EQUAL 2)

install(DIRECTORY
    ${CASCADE_DIRS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/assets
)
