include(ExternalProject)

set(ITRACKER itracker)
message(STATUS "Configuring ${ITRACKER}")

# Find packages
set(OpenCV_COMPONENTS core)
find_package(OpenCV ${OpenCV_VERSION} REQUIRED COMPONENTS ${OpenCV_COMPONENTS})
if(NOT OpenCV_FOUND)
    message(WARNING "Can not build ${ITRACKER} library, OpenCV was not found.")
    return()
endif(NOT OpenCV_FOUND)

# Brew OpenBLAS paths
set(BLAS_INCLUDE_DIRS /usr/local/opt/openblas/include)
set(BLAS_LIBRARY_DIRS /usr/local/opt/openblas/lib)
set(BLAS_LIBS openblas)

# Download & build caffe
set(CAFFE_PROJECT caffe-project)
ExternalProject_Add(${CAFFE_PROJECT}
    GIT_REPOSITORY https://github.com/BVLC/caffe.git
    GIT_TAG 1.0
    CONFIGURE_COMMAND cp "<SOURCE_DIR>/Makefile.config.example" "<SOURCE_DIR>/Makefile.config"
    BUILD_COMMAND cd <SOURCE_DIR> && make BLAS=open BLAS_LIB="${BLAS_LIBRARY_DIRS}" BLAS_INCLUDE="${BLAS_INCLUDE_DIRS}" CPU_ONLY=1 OPENCV_VERSION=3 all
    INSTALL_COMMAND ""
)

# Set paths to build results
ExternalProject_Get_Property(${CAFFE_PROJECT} source_dir)
set(CAFFE_ROOT_DIR ${source_dir})
set(CAFFE_INCLUDE_DIRS ${CAFFE_ROOT_DIR}/include ${CAFFE_ROOT_DIR}/.build_release/src)
set(CAFFE_LIBRARY_DIRS ${CAFFE_ROOT_DIR}/build/lib)
set(CAFFE_LIBS caffe boost_system glog ${BLAS_LIBS})

# Build caffe implementation
add_definitions(-DCPU_ONLY)

# Compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2")

# Set includes and links
include_directories(
    ${CAFFE_INCLUDE_DIRS}
    ${BLAS_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)
link_directories(
    ${CAFFE_LINK_DIRS}
    ${BLAS_LIBRARY_DIRS}
    ${OpenCV_LIBRARY_DIRS}
)

# Library target
add_library(${ITRACKER}
    ${CMAKE_CURRENT_SOURCE_DIR}/itracker.cpp
)
add_dependencies(${ITRACKER} ${CAFFE_PROJECT})

target_link_libraries(${ITRACKER} ${CAFFE_LIBS} ${BLAS_LIBS} ${OpenCV_LIBRARIES})

target_include_directories(${ITRACKER} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
set_target_properties(${ITRACKER}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

install(TARGETS ${ITRACKER}
        EXPORT ${ITRACKER}
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/${ITRACKER}
        DESTINATION include)

# Install CMake files to include gaze in other projects
set(CMAKE_LIB_PATH lib/cmake/${ITRACKER})
install(EXPORT ${ITRACKER}
        NAMESPACE ${ITRACKER}::
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_LIB_PATH})

configure_file(${CMAKE_SOURCE_DIR}/cmake/${ITRACKER}Config.cmake.in
               config/${ITRACKER}Config.cmake @ONLY)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    config/${ITRACKER}ConfigVersion.cmake
    VERSION 1.0
    COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config/${ITRACKER}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/config/${ITRACKER}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_LIB_PATH})
