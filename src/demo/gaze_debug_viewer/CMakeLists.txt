# Debug viewer: Just show input stream

message(STATUS "Configuring demo gaze_debug_viewer.")

if(NOT Boost_VERSION)
    cmake_minimum_required(VERSION 3.9.1)
    set(Boost_VERSION 1.65.0)
endif(NOT Boost_VERSION)
set(Boost_COMPONENTS program_options)


find_package(Boost ${Boost_VERSION} COMPONENTS ${Boost_COMPONENTS})

if(NOT Boost_FOUND)
    message(WARNING "Can not build gaze_debug_viewer example, did not find Boost.")
    return()
endif(NOT Boost_FOUND)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(SIMPLE_VIEWER_TARGET gaze_debug_viewer)
add_executable(${SIMPLE_VIEWER_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/gaze_debug_viewer.cpp)
set_target_properties(${SIMPLE_VIEWER_TARGET}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
if(Boost_TARGET)
    add_dependencies(${SIMPLE_VIEWER_TARGET} ${Boost_TARGET})
endif(Boost_TARGET)
target_link_libraries(${SIMPLE_VIEWER_TARGET} ${PROJECT_NAME} ${Boost_LIBRARIES})
