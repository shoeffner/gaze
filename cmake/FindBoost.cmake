include(ExternalProject)
include(SearchSystemPackage)

message(STATUS "Configuring Boost")
maybe_find_system_package_and_return()
message(STATUS "Setting up Boost download and build process.")

##
## SET VARIABLES
##

if(NOT BOOST_ROOT)
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/boost)
endif(NOT BOOST_ROOT)
if(Boost_DEBUG)
    message(STATUS "BOOST_ROOT=" ${BOOST_ROOT})
endif(Boost_DEBUG)

if(NOT BOOST_SOURCE_DIR)
    set(BOOST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor/boost)
endif(NOT BOOST_SOURCE_DIR)
if(Boost_DEBUG)
    message(STATUS "BOOST_SOURCE_DIR=" ${BOOST_SOURCE_DIR})
endif(Boost_DEBUG)


##
## BUILD COMMANDS
##


# Base commands
list(APPEND _BOOST_BUILD_OPTIONS -j4 variant=release link=static)
if(NOT Boost_DEBUG)
    list(APPEND _BOOST_BUILD_OPTIONS -d0)
else(NOT Boost_DEBUG)
    list(APPEND _BOOST_BUILD_OPTIONS -d+2)
endif(NOT Boost_DEBUG)

set(_BOOST_CONFIGURE_COMMAND ./bootstrap.sh --prefix=${BOOST_ROOT})
set(_BOOST_BUILD_COMMAND ./b2 ${_BOOST_BUILD_OPTIONS} && ./b2 ${_BOOST_BUILD_OPTIONS} headers)
set(_BOOST_INSTALL_COMMAND ./b2 ${_BOOST_BUILD_OPTIONS} install)

# Add components to configure
if(Boost_FIND_COMPONENTS)
    string(REPLACE ";" "," _BOOST_LIBRARIES "${Boost_FIND_COMPONENTS}")
    set(_BOOST_CONFIGURE_COMMAND ${_BOOST_CONFIGURE_COMMAND} --with-libraries=${_BOOST_LIBRARIES})
endif(Boost_FIND_COMPONENTS)


if(Boost_DEBUG)
    string(REPLACE ";" " " _DEBUG_BOOST_CONFIGURE_COMMAND "${_BOOST_CONFIGURE_COMMAND}")
    string(REPLACE ";" " " _DEBUG_BOOST_BUILD_COMMAND "${_BOOST_BUILD_COMMAND}")
    string(REPLACE ";" " " _DEBUG_BOOST_INSTALL_COMMAND "${_BOOST_INSTALL_COMMAND}")
    message(STATUS "Boost configure command: " ${_DEBUG_BOOST_CONFIGURE_COMMAND})
    message(STATUS "Boost build command: " ${_DEBUG_BOOST_BUILD_COMMAND})
    message(STATUS "Boost install command: " ${_DEBUG_BOOST_INSTALL_COMMAND})
endif(Boost_DEBUG)


##
## GIT SETTINGS
##

set(_BOOST_GIT_TAG "master")
if(Boost_FIND_VERSION)
    set(_BOOST_GIT_TAG "boost-${Boost_FIND_VERSION}")
endif(Boost_FIND_VERSION)


##
## SETUP EXTERNAL PROJECT
##

set(Boost_TARGET boost)
ExternalProject_Add(${Boost_TARGET}
    GIT_REPOSITORY "https://github.com/boostorg/boost.git"
    GIT_TAG ${_BOOST_GIT_TAG}

    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_COMMAND ""

    SOURCE_DIR "${BOOST_SOURCE_DIR}"

    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND "${_BOOST_CONFIGURE_COMMAND}"
    BUILD_COMMAND "${_BOOST_BUILD_COMMAND}"
    INSTALL_COMMAND "${_BOOST_INSTALL_COMMAND}"
)


##
## SET BOOST VARIABLES
##

set(Boost_FOUND True)
set(Boost_LIBRARY_DIRS ${BOOST_ROOT}/lib)
set(Boost_INCLUDE_DIRS ${BOOST_ROOT}/include)
foreach(LIBRARY ${Boost_FIND_COMPONENTS})
    list(APPEND Boost_LIBRARIES boost_${LIBRARY})
endforeach(LIBRARY)

if(Boost_DEBUG)
    message(STATUS "Boost_FOUND=" ${Boost_FOUND})
    message(STATUS "Boost_LIBRARY_DIRS=" ${Boost_LIBRARY_DIRS})
    message(STATUS "Boost_INCLUDE_DIRS=" ${Boost_INCLUDE_DIRS})
    message(STATUS "Boost_LIBRARIES=" ${Boost_LIBRARIES})
endif(Boost_DEBUG)
