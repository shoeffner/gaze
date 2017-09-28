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
    ${CMAKE_CURRENT_SOURCE_DIR}/gaze_tracker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source_capture.cpp
)

add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
target_link_libraries(${WPL_TARGET} ${PROJECT_NAME} ${GTK_LIBRARIES} ${Boost_LIBRARIES})
if(${OpenCV_TARGET})
    add_dependencies(${PROJECT_NAME} ${OpenCV_TARGET})
endif(${OpenCV_TARGET})

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBRARIES}
)
