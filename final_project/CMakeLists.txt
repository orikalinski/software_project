cmake_minimum_required(VERSION 3.6)
project(final_project)

set(CMAKE_CXX_FLAGS -std=c++11)

add_custom_target(
        final_project
        COMMAND make
        WORKING_DIRECTORY /home/ori/computer_sience/software_project/final_project
)

