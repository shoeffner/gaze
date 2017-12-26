include(ExternalProject)

# Test files
list(APPEND TEST_FILES
    pipeline_steps/pupil_localization.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)


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
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)
add_dependencies(${TEST_TARGET} ${DOCTEST})
target_link_libraries(${TEST_TARGET} ${PROJECT_NAME})

add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
                   COMMAND
                   ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/tests/assets
                   $<TARGET_FILE_DIR:${TEST_TARGET}>/assets)
add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
                   COMMAND
                   ${CMAKE_COMMAND} -E touch
                   $<TARGET_FILE_DIR:${TEST_TARGET}>/gaze.yaml)
add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
                   COMMAND
                   ${CMAKE_COMMAND} -E copy
                   ${CMAKE_BINARY_DIR}/shape_predictor_5_face_landmarks.dat
                   $<TARGET_FILE_DIR:${TEST_TARGET}>/shape_predictor_5_face_landmarks.dat)
add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
                   COMMAND
                   ${CMAKE_COMMAND} -E copy
                   ${CMAKE_BINARY_DIR}/shape_predictor_68_face_landmarks.dat
                   $<TARGET_FILE_DIR:${TEST_TARGET}>/shape_predictor_68_face_landmarks.dat)
