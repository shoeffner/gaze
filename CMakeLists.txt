cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(Boost_DEBUG ON)
project(gaze)

find_package(Boost 1.65.0 REQUIRED COMPONENTS program_options)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
