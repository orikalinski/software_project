cmake_minimum_required(VERSION 3.6)
project(opencv)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)

set(SOURCE_FILES sp_image_proc_util.cpp SPPoint.c)
add_executable(opencv ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv ${OpenCV_LIBS})