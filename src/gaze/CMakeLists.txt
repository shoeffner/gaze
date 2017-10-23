include(ExternalProject)

message(STATUS "Configuring gaze")


# Find packages
set(OpenCV_COMPONENTS core videoio)
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})
if(NOT OpenCV_FOUND)
    message(WARNING "Can not build gaze library, OpenCV was not found.")
    return()
endif(NOT OpenCV_FOUND)

find_package(dlib ${dlib_VERSION} REQUIRED)
if(NOT dlib_FOUND)
    message(WARNING "Can not build gaze library, dlib was not found.")
    return()
endif(NOT dlib_FOUND)

# Includes and libraries
include_directories(
    ${dlib_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)
link_directories(
    ${dlib_LIBRARY_DIRS}
    ${OpenCV_LIBRARY_DIRS}
)


# Library
list(APPEND LIBRARY_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gaze_tracker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/debug_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/event_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/pipeline_step_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_step.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/face_landmarks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/pupil_localization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/source_capture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/utility.cpp
)


# Landmarks file for face detection
set(LANDMARKS_FILE shape_predictor_5_face_landmarks.dat)
ExternalProject_Add(${LANDMARKS_FILE}
    URL "https://github.com/davisking/dlib-models/raw/master/shape_predictor_5_face_landmarks.dat.bz2"
    URL_HASH "SHA256=6e787bbebf5c9efdb793f6cd1f023230c4413306605f24f299f12869f95aa472"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND bunzip2 -ck <SOURCE_DIR>/../shape_predictor_5_face_landmarks.dat.bz2 > ${CMAKE_BINARY_DIR}/${LANDMARKS_FILE}
    INSTALL_COMMAND ""
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_NO_EXTRACT 1
)


# Compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2")

# Library target
add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} ${LANDMARKS_FILE})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME}
    ${dlib_LIBRARIES}
    ${OpenCV_LIBRARIES}
)
