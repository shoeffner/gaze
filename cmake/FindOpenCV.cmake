include(ExternalProject)

message(STATUS "Configuring OpenCV")


##
## SEARCH FOR SYSTEM OPENCV
##

message(STATUS "Checking for system OpenCV")
find_package(OpenCV ${OpenCV_FIND_VERSION} QUIET NO_MODULE COMPONENTS ${OpenCV_FIND_COMPONENTS})
if(OpenCV_FOUND)
    message(STATUS "System OpenCV found")
    return()
else(OpenCV_FOUND)
    message(STATUS "OpenCV was not found on the system, will download it")
endif(OpenCV_FOUND)


##
## SET VARIABLES
##

if(NOT OPENCV_ROOT)
    set(OPENCV_ROOT ${CMAKE_BINARY_DIR}/opencv)
endif(NOT OPENCV_ROOT)
set(OPENCV_INSTALL_DIR ${OPENCV_ROOT}/install)
if(OpenCV_DEBUG)
    message(STATUS OPENCV_ROOT=${OPENCV_ROOT})
    message(STATUS OPENCV_INSTALL_DIR=${OPENCV_INSTALL_DIR})
endif(OpenCV_DEBUG)

if(NOT OPENCV_SOURCE_DIR)
    set(OPENCV_SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor/opencv)
endif(NOT OPENCV_SOURCE_DIR)
if(OpenCV_DEBUG)
    message(STATUS OPENCV_SOURCE_DIR=${OPENCV_SOURCE_DIR})
endif(OpenCV_DEBUG)



##
## BUILD COMMANDS
##

list(APPEND _OPENCV_DISABLED_COMPONENTS
    apps calib3d core dnn features2d flann highgui imgcodecs imgcodecs imgproc
    ml objdetect photo shape stitching superres video videoio videostab)

list(APPEND _OPENCV_CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=RELEASE
    -DBUILD_SHARED_LIBS=ON
    -DBUILD_DOCS=OFF
    -DBUILD_PERF_TESTS=OFF
    -DBUILD_TESTS=OFF
    -DBUILD_EXAMPLES=OFF
)

# Add components to configure
if(OpenCV_FIND_COMPONENTS)
    foreach(COMPONENT ${OpenCV_FIND_COMPONENTS})
        list(APPEND _OPENCV_CMAKE_ARGS -DBUILD_opencv_${COMPONENT}=ON)
    endforeach(COMPONENT)

    list(REMOVE_ITEM _OPENCV_DISABLED_COMPONENTS ${OpenCV_FIND_COMPONENTS})
    foreach(COMPONENT ${_OPENCV_DISABLED_COMPONENTS})
        list(APPEND _OPENCV_CMAKE_ARGS -DBUILD_opencv_${COMPONENT}=OFF)
    endforeach(COMPONENT)
endif(OpenCV_FIND_COMPONENTS)

if(OpenCV_DEBUG)
    string(REPLACE ";" " " _DEBUG_OPENCV_CMAKE_ARGS "${_OPENCV_CMAKE_ARGS}")
    message(STATUS "OpenCV CMAKE Arguments: ${_DEBUG_OPENCV_CMAKE_ARGS}")
endif(OpenCV_DEBUG)


##
## GIT SETTINGS
##

set(_OPENCV_GIT_TAG "master")
if(OpenCV_FIND_VERSION)
    set(_OPENCV_GIT_TAG "${OpenCV_FIND_VERSION}")
endif(OpenCV_FIND_VERSION)


##
## SETUP EXTERNAL PROJECT
##

ExternalProject_Add(opencv
    GIT_REPOSITORY "https://github.com/opencv/opencv.git"
    GIT_TAG ${_OPENCV_GIT_TAG}

    UPDATE_COMMAND ""
    PATCH_COMMAND ""

    SOURCE_DIR "${OPENCV_SOURCE_DIR}"
    BINARY_DIR "${OPENCV_ROOT}"

    CMAKE_ARGS ${_OPENCV_CMAKE_ARGS}

    BUILD_IN_SOURCE 0
    BUILD_COMMAND make -j4
    INSTALL_COMMAND make install
)


##
## SET OpenCV VARIABLES
##

set(OpenCV_FOUND True)
set(OpenCV_LIBRARY_DIRS ${OPENCV_INSTALL_DIR}/lib)
set(OpenCV_INCLUDE_DIRS ${OPENCV_INSTALL_DIR}/include)
set(OpenCV_DIR ${OPENCV_INSTALL_DIR}/share/OpenCV)
foreach(LIBRARY ${OpenCV_FIND_COMPONENTS})
    list(APPEND OpenCV_LIBRARIES opencv_${LIBRARY})
endforeach(LIBRARY)

if(OpenCV_DEBUG)
    message(STATUS "OpenCV_FOUND=" ${OpenCV_FOUND})
    message(STATUS "OpenCV_LIBRARY_DIRS=" ${OpenCV_LIBRARY_DIRS})
    message(STATUS "OpenCV_INCLUDE_DIRS=" ${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV_LIBRARIES=" ${OpenCV_LIBRARIES})
    message(STATUS "OpenCV_DIR=" ${OpenCV_DIR})
endif(OpenCV_DEBUG)


##
## UPDATE PROJECT DEPENDENCY LIST
##

list(APPEND ${PROJECT_NAME}_DEPENDENCIES opencv)
