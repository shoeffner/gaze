# Create to-do list if `ack` is available
find_program(ACK NAMES ack)
if(ACK)
    add_custom_target(todo
                      COMMAND ${ACK} TODO
                              cmake include src
                              --ignore-file=is:TodoTarget.cmake
                              >
                              ${CMAKE_SOURCE_DIR}/TODO
                      BYPRODUCTS ${CMAKE_SOURCE_DIR}/TODO
                      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_dependencies(${PROJECT_NAME} todo)
endif(ACK)
