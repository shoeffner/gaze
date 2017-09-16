# Where People Look, Judd et al. 2009

message(STATUS "Configuring demo where_people_look.")

find_package(PkgConfig)
pkg_check_modules(GTK gtk+-3.0)

if(GTK_FOUND)
    include_directories(${GTK_INCLUDE_DIRS})
    link_directories(${GTK_LINK_DIRS})

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

    add_executable(${WPL_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/where_people_look.cpp)
    target_link_libraries(${WPL_TARGET} ${PROJECT_NAME} ${GTK_LIBRARIES})

    # Install stimuli
    add_dependencies(${WPL_TARGET} ${WPL_STIMULI})
    install(DIRECTORY ${${WPL_STIMULI}_SOURCE_DIR}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
else(GTK_FOUND)
    message(WARNING "Can not build where_people_look example, did not find GTK 3.")
endif(GTK_FOUND)
