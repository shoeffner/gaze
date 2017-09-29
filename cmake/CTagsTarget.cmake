# Build tags file if available
find_program(CTAGS NAMES ctags)
if(CTAGS)
    set(CTAGS_FILE tags)
    add_custom_target(${CTAGS_FILE}
                       COMMAND ${CTAGS} -R -f ${CTAGS_FILE}
                               ${CMAKE_SOURCE_DIR}/src
                               ${CMAKE_SOURCE_DIR}/include
                       BYPRODUCTS ${CTAGS_FILE}
                       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/.git)
    add_dependencies(${PROJECT_NAME} ${CTAGS_FILE})
endif(CTAGS)
