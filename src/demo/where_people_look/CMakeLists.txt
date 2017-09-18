# Where People Look, Judd et al. 2009

message(STATUS "Configuring demo where_people_look.")


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

set(WPL_RESOURCES_FILE ${CMAKE_CURRENT_BINARY_DIR}/wpl_gresources.c)
# Compile gresources
add_custom_command(
    OUTPUT ${WPL_RESOURCES_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES} --target=${WPL_RESOURCES_FILE} --generate-source ${WPL_TARGET}.gresource.xml --)


# Executable
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(${WPL_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/where_people_look.cpp ${WPL_RESOURCES_FILE})
target_link_libraries(${WPL_TARGET} ${PROJECT_NAME} ${GTK_LIBRARIES})


# Install stimuli
add_dependencies(${WPL_TARGET} ${WPL_STIMULI})
install(DIRECTORY ${${WPL_STIMULI}_SOURCE_DIR}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
