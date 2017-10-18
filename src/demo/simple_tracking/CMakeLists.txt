message(STATUS "Configuring demo simple_tracking.")

# Target variables
set(ST_TARGET simple_tracking)

set(OpenCV_COMPONENTS core highgui imgproc)
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})
if (NOT OpenCV_FOUND)
    message(WARNING "Can not build demo simple_tracking, OpenCV not found.")
    return()
endif(NOT OpenCV_FOUND)

# Compile executable
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
