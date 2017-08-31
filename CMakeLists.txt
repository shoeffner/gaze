cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "gaze")
project(${PROJECT_NAME})

find_package(OpenCV REQUIRED)

add_executable( ${PROJECT_NAME} ${PROJECT_NAME}.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX})
