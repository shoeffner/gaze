cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(gaze)


# Find packages
find_package(Boost 1.65.0 REQUIRED COMPONENTS program_options)


# Includes and libraries
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


# Executable and dependencies
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})


# Linking
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


# Installation
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
