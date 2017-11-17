include(ExternalProject)

message(STATUS "Configuring ${PROJECT_NAME}")


# Find packages
set(OpenCV_COMPONENTS core videoio calib3d)
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})
if(NOT OpenCV_FOUND)
    message(WARNING "Can not build ${PROJECT_NAME} library, OpenCV was not found.")
    return()
endif(NOT OpenCV_FOUND)

find_package(dlib ${dlib_VERSION} REQUIRED)
if(NOT dlib_FOUND)
    message(WARNING "Can not build ${PROJECT_NAME} library, dlib was not found.")
    return()
endif(NOT dlib_FOUND)


# Custom "find_package" for yaml-cpp
set(YAML_CPP yaml-cpp)
ExternalProject_Add(${YAML_CPP}
    GIT_REPOSITORY "https://github.com/jbeder/${YAML_CPP}.git"
    GIT_TAG "master"
    GIT_SHALLOW 1
    CMAKE_ARGS
        -DYAML_CPP_BUILD_TESTS=OFF
        -DYAML_CPP_BUILD_TOOLS=OFF
        -DCMAKE_INSTALL_MESSAGE=LAZY
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(${YAML_CPP} install_dir)
set(YAML_CPP_INCLUDE_DIRS ${install_dir}/include)
set(YAML_CPP_LIBRARIES ${install_dir}/lib/lib${YAML_CPP}.a)
set(YAML_CPP_LIBRARY_DIRS ${install_dir}/lib)


# Includes and libraries
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)
link_directories(
    ${OpenCV_LIBRARY_DIRS}
    ${YAML_CPP_LIBRARY_DIRS}
)


# Read default configuration
set(DEFAULT_CONFIG_SRC ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.default.yaml)
file(READ ${DEFAULT_CONFIG_SRC} DEFAULT_CONFIGURATION_FILE)
string(REGEX REPLACE "\n" "\\\\n\"\n\"" DEFAULT_CONFIGURATION_FILE ${DEFAULT_CONFIGURATION_FILE})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${DEFAULT_CONFIG_SRC})
configure_file(util/config.in.cpp util/config.cpp)

# Add files to compile
list(APPEND LIBRARY_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gaze_tracker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/debug_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/event_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_step.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/face_landmarks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/gaze_point_calculation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/head_pose_estimation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/pupil_localization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_steps/source_capture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/data.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/util/config.cpp
)


# Landmarks file for face detection
set(LANDMARKS_FILE shape_predictor_5_face_landmarks.dat)
ExternalProject_Add(${LANDMARKS_FILE}
    URL "https://github.com/davisking/dlib-models/raw/master/${LANDMARKS_FILE}.bz2"
    URL_HASH "SHA256=6e787bbebf5c9efdb793f6cd1f023230c4413306605f24f299f12869f95aa472"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND bunzip2 -ck <SOURCE_DIR>/../${LANDMARKS_FILE}.bz2 > ${CMAKE_BINARY_DIR}/${LANDMARKS_FILE}
    INSTALL_COMMAND ""
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_NO_EXTRACT 1
)


# Using 68 landmarks for the head orientation for now
set(LANDMARKS_FILE_68 shape_predictor_68_face_landmarks.dat)
ExternalProject_Add(${LANDMARKS_FILE_68}
    URL "https://github.com/davisking/dlib-models/raw/master/${LANDMARKS_FILE_68}.bz2"
    URL_HASH "SHA256=7d6637b8f34ddb0c1363e09a4628acb34314019ec3566fd66b80c04dda6980f5"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND bunzip2 -ck <SOURCE_DIR>/../${LANDMARKS_FILE_68}.bz2 > ${CMAKE_BINARY_DIR}/${LANDMARKS_FILE_68}
    INSTALL_COMMAND ""
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_NO_EXTRACT 1
)


# Compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2")

# Library target
add_library(${PROJECT_NAME} ${LIBRARY_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} ${LANDMARKS_FILE} ${LANDMARKS_FILE_68} ${YAML_CPP})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME}
    dlib::dlib
    ${OpenCV_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}
        DESTINATION include)
