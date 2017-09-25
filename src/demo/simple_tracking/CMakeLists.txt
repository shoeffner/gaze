message(STATUS "Configuring demo simple_tracking.")

# Target variables
set(ST_TARGET simple_tracking)

set(OpenCV_COMPONENTS core highgui)
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})

# Compile executable
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)
link_directories(
    ${OpenCV_LIBRARY_DIRS}
)

add_executable(${ST_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_tracking.cpp
)
set_target_properties(${ST_TARGET}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
target_link_libraries(${ST_TARGET}
    ${PROJECT_NAME}
    ${OpenCV_LIBRARIES}
)
if(${OpenCV_TARGET})
    add_dependencies(${ST_TARGET} ${OpenCV_TARGET})
endif(${OpenCV_TARGET})
