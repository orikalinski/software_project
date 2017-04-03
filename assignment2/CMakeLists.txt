cmake_minimum_required (VERSION 2.6)
project (SP2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
#set(SOURCE_FILES SPPoint.c main.c   )
set(SOURCE_FILES SPBPriorityQueue.c unit_tests/sp_bpqueue_unit_test.c)
#set(SOURCE_FILES 313306490_200672954_assignment2/SPBPriorityQueue.c 313306490_200672954_assignment2/unit_tests/sp_bpqueue_unit_test.c)
add_executable(SP2 ${SOURCE_FILES})