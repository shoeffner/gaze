message(STATUS "Adding examples from ${CMAKE_CURRENT_SOURCE_DIR}.")

file(GLOB DEMOS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(DEMO ${DEMOS})
    if(IS_DIRECTORY ${DEMO})
        ADD_SUBDIRECTORY(${DEMO})
    endif(IS_DIRECTORY ${DEMO})
endforeach(DEMO)

message(STATUS "Examples processed.")
