# Where People Look, Judd et al. 2009

message(STATUS "Configuring demo where_people_look.")


if(NOT Boost_VERSION)
    cmake_minimum_required(VERSION 3.9.1)
    set(Boost_VERSION 1.65.0)
endif(NOT Boost_VERSION)

find_package(Boost ${Boost_VERSION} COMPONENTS filesystem)
if(NOT Boost_FOUND)
  message(WARNING "Can not build where_people_look example, did not find Boost.")
  return()
endif(NOT Boost_FOUND)


# PkgConfig
find_package(PkgConfig)


# GTK
pkg_check_modules(GTK gtk+-3.0)
if(NOT GTK_FOUND)
    message(WARNING "Can not build where_people_look example, did not find GTK 3.")
    return()
endif(NOT GTK_FOUND)


# GLIB
pkg_check_modules(GLIB glib-2.0)
find_program(GLIB_COMPILE_RESOURCES
             NAMES glib-compile-resources
             HINTS ${GLIB_PREFIX})
if(NOT GLIB_COMPILE_RESOURCES)
    message(WARNING "Can not build where_people_look example, did not find program glib-compile-resources.")
    return()
endif(NOT GLIB_COMPILE_RESOURCES)


# Target variables
set(WPL_TARGET where_people_look)
set(WPL_STIMULI wpl_stimuli)

# Download stimuli
set(${WPL_STIMULI}_SOURCE_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/${WPL_STIMULI}-prefix/src/${WPL_STIMULI}")
ExternalProject_Add(${WPL_STIMULI}
    URL "http://people.csail.mit.edu/tjudd/WherePeopleLook/ALLSTIMULI.zip"
    URL_HASH "SHA256=2bd451ea3aed9422c463d5cd47f9bf65d1fec53b458afb5ef65640a2a8857788"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)


# Compile gresources
# Find dependency files and make proper list
execute_process(COMMAND
    ${GLIB_COMPILE_RESOURCES} --generate-dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/${WPL_TARGET}.gresource.xml
    OUTPUT_VARIABLE WPL_RESOURCES
)
string(REPLACE "\n" ";" WPL_RESOURCES ${WPL_RESOURCES})
list(APPEND WPL_RESOURCES ${WPL_TARGET}.gresource.xml)
# Prepend full paths
foreach(WPL_RESOURCE ${WPL_RESOURCES})
  list(REMOVE_ITEM WPL_RESOURCES ${WPL_RESOURCE})
  list(APPEND WPL_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${WPL_RESOURCE})
endforeach(WPL_RESOURCE)

# Create gresources compilation command
set(WPL_RESOURCES_C ${CMAKE_CURRENT_BINARY_DIR}/wpl_gresources.c)
add_custom_command(
    OUTPUT ${WPL_RESOURCES_C}
    DEPENDS ${WPL_RESOURCES} ${WPL_RESOURCES_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES} --target=${WPL_RESOURCES_C}
            --generate-source
            --internal
            ${CMAKE_CURRENT_SOURCE_DIR}/${WPL_TARGET}.gresource.xml
)


# Compile executable
include_directories(${GTK_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
link_directories(${GTK_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})

add_executable(${WPL_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/where_people_look.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui_cb.cpp
    ${WPL_RESOURCES_C})
set_target_properties(${WPL_TARGET}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
target_link_libraries(${WPL_TARGET} ${PROJECT_NAME} ${GTK_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(${WPL_TARGET} ${WPL_STIMULI})


# Install stimuli
install(DIRECTORY ${${WPL_STIMULI}_SOURCE_DIR}
        DESTINATION "${CMAKE_BINARY_DIR}")
