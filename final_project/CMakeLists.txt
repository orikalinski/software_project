cmake_minimum_required(VERSION 3.6)
project(final_project)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES SPLogger.c unit_tests/sp_logger_unit_test.c)
set(SOURCE_FILES sp_utils.c SPPoint.c SPKDArray.c SPKDTree.c)
add_executable(final_project ${SOURCE_FILES})

target_link_libraries(final_project m)