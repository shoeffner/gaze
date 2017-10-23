# Generate documentation

find_program(DOXYGEN NAMES doxygen)
if(NOT DOXYGEN)
    message(WARNING "Can not create documentation: Can't find `doxygen` executable.")
    return()
endif(NOT DOXYGEN)

include(ExternalProject)

list(APPEND DOXYFILES Doxyfile)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gh-pages)

# Set up gh-pages worktree
if(NOT IS_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
    message(STATUS "Creating git worktree to checkout gh-pages properly.")
    execute_process(COMMAND git worktree add gh-pages gh-pages
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif(NOT IS_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})


# Get current git branch
execute_process(COMMAND git branch
                COMMAND grep *
                COMMAND cut -f 2 -d " "
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_CURRENT_BRANCH
                OUTPUT_STRIP_TRAILING_WHITESPACE)


# Generate develop options
if(NOT "master" STREQUAL "${GIT_CURRENT_BRANCH}")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYFILE_DEVELOP ${CMAKE_BINARY_DIR}/Doxyfile.develop)

    file(WRITE ${DOXYFILE_DEVELOP} "OUTPUT_DIRECTORY = ${DOXYGEN_OUTPUT_DIRECTORY}\n")
    file(APPEND ${DOXYFILE_DEVELOP} "GENERATE_TAGFILE = ${DOXYGEN_OUTPUT_DIRECTORY}/gaze.tag\n")

    list(APPEND DOXYFILES ${DOXYFILE_DEVELOP})
endif(NOT "master" STREQUAL "${GIT_CURRENT_BRANCH}")

message(STATUS "Documentation will be written to ${DOXYGEN_OUTPUT_DIRECTORY}")


# Skip tagfiles if needed.
option(DOXYGEN_TAGFILES "Include external tagfiles." OFF)
if(NOT DOXYGEN_TAGFILES)
    message(STATUS "Skipping external documentation links.")
    set(DOXYFILE_NO_TAGFILES ${CMAKE_BINARY_DIR}/Doxyfile.clear_tagfiles)
    file(WRITE ${DOXYFILE_NO_TAGFILES} "TAGFILES = \n")
    list(APPEND DOXYFILES ${DOXYFILE_NO_TAGFILES})
endif(NOT DOXYGEN_TAGFILES)


# Generate clang parameters
if(NOT WIN32)
    find_program(PYTHON NAMES python3)
    if(PYTHON)
        set(DOXYFILE_CLANG Doxyfile.clang)
        add_custom_command(OUTPUT ${DOXYFILE_CLANG}
                           COMMAND ${PYTHON}
                               ${CMAKE_SOURCE_DIR}/cmake/compile_commands_to_doxyfile_clang.py
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        list(APPEND DOXYFILES ${CMAKE_BINARY_DIR}/${DOXYFILE_CLANG})
    endif(PYTHON)
endif(NOT WIN32)


# Add external tag files for doxygen
set(LIBSTDCPP_TAG libstdc++.tag)
ExternalProject_Add(${LIBSTDCPP_TAG}
    URL "https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/libstdc++.tag"
    URL_HASH "SHA256=f6c8c74e8293aefaefb4d25fd154d5f537b3bc80e7ceecaa02c5a01836fc09e6"
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/doxy-tags
    DOWNLOAD_NAME ${LIBSTDCPP_TAG}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_NO_EXTRACT 1
)


# Set doxygen command
if(NOT WIN32)
    # Combine all DOXYFILES into "one" virtual file and pass it to doxygen via stdin
    set(DOXYGEN_COMMAND cat ${DOXYFILES} | ${DOXYGEN} -)
else(NOT WIN32)
    message(WARNING "Currently, the windows documentation generation is limited. Be careful!")
    set(DOXYGEN_COMMAND ${DOXYGEN} Doxyfile)
endif(NOT WIN32)


set(DOCS_TARGET docs)
add_custom_target(${DOCS_TARGET}
                  DEPENDS ${DOXYFILE_CLANG}
                          ${LIBSTDCPP_TAG})
add_custom_command(TARGET ${DOCS_TARGET}
                   COMMAND ${DOXYGEN_COMMAND}
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
