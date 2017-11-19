include(ExternalProject)

# Test files
list(APPEND TEST_FILES
    pipeline_steps/pupil_localization.cpp
)


# doctest
set(DOCTEST doctest)
ExternalProject_Add(${DOCTEST}
    GIT_REPOSITORY "https://github.com/onqtam/${DOCTEST}.git"
    GIT_TAG "1.2.6"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(${DOCTEST} install_dir)
include_directories(${install_dir}/src/doctest)


# Generate and add main file to compile files
set(TEST_MAIN testmain.cpp)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${TEST_MAIN}
    "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n#include \"doctest/doctest.h\"")
list(APPEND TEST_FILES ${CMAKE_CURRENT_BINARY_DIR}/${TEST_MAIN})


# Compile test target
set(TEST_TARGET ${PROJECT_NAME}_test)
add_executable(${TEST_TARGET} ${TEST_FILES})
set_target_properties(${TEST_TARGET}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
add_dependencies(${TEST_TARGET} ${DOCTEST})
target_link_libraries(${TEST_TARGET} ${PROJECT_NAME})
