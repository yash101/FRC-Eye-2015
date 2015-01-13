project(VisionServer2015)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
file(GLOB_RECURSE DEVLIB_HEADERS "DevLib/*.hpp")
file(GLOB_RECURSE DEVLIB_SOURCES "DevLib/*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")
file(GLOB_RECURSE SOURCES "*.cpp")

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${DEVLIB_HEADERS} ${DEVLIB_SOURCES} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
