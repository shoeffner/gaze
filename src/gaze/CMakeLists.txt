if(NOT OpenCV_VERSION)
    set(OpenCV_VERSION 3.3.0)
endif(NOT OpenCV_VERSION)
set(OpenCV_COMPONENTS core highgui imgcodecs imgproc videoio objdetect)

if(NOT dlib_VERSION)
    set(dlib_VERSION 19.7)
endif(NOT dlib_VERSION)


# Find packages
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})
find_package(X11)
find_package(dlib ${dlib_VERSION} REQUIRED)
find_package(lapack REQUIRED)
find_package(blas REQUIRED)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_step.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/face_landmarks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/pupil_localization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/source_capture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/utility.cpp
)


# Compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse2")


# Library target

add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
if(${OpenCV_TARGET})
    add_dependencies(${PROJECT_NAME} ${OpenCV_TARGET})
endif(${OpenCV_TARGET})

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBRARIES}
    ${dlib_LIBS}
    ${LAPACK_LIBRARIES}
    ${BLAS_LIBRARIES}
    ${X11_LIBRARIES}
)
