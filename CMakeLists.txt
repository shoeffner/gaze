# Version settings, general settings
cmake_minimum_required(VERSION 3.1.3)
set(CMAKE_CXX_STANDARD 98)

project(${PROJECT_NAME})

# Executable
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX})
install(DIRECTORY "${OpenCV_DIR}/install/share/OpenCV/haarcascades" DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX}/res)

# Libraries and linking
find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

