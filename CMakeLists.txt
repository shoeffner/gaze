# Version settings, general settings
cmake_minimum_required(VERSION 3.9.1) # Boost 1.65 requires 3.9.1
set(CMAKE_CXX_STANDARD 98)

project(${PROJECT_NAME})

# Find libraries
find_package(OpenCV REQUIRED)
find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


# Include directories
include_directories(${Boost_INCLUDE_DIR})

include_directories(${Boost_LIBRARY_DIR})


# Executable
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)


# Linking
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


# Installation
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX})
install(DIRECTORY "${OpenCV_DIR}/install/share/OpenCV/haarcascades" DESTINATION ${${PROJECT_NAME}_INSTALL_PREFIX}/res)
