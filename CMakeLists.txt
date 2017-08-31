# Version settings, general settings
cmake_minimum_required(VERSION 3.1.3)
set(CMAKE_CXX_STANDARD 98)

project(${PROJECT_NAME})

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX})

# Libraries and linking
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS lambda)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBS})
