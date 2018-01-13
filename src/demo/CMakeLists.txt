message(STATUS "Adding examples from ${CMAKE_CURRENT_SOURCE_DIR}.")

if(WITH_CAFFE)
    link_directories(
        ${CMAKE_BINARY_DIR}/src/itracker/caffe-project-prefix/src/caffe-project/build/lib
        /usr/local/opt/openblas/lib
    )
endif(WITH_CAFFE)

file(GLOB DEMOS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(DEMO ${DEMOS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DEMO})
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${DEMO})
    endif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DEMO})
endforeach(DEMO)

message(STATUS "Examples processed.")
