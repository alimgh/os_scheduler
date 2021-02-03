cmake_minimum_required(VERSION 3.17)
project(scheduler C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS -pthread)
#set(CMAKE_C_FLAGS -ltr)

add_executable(sjf sjf_test.c scheduler.c scheduler.h sjf.c sjf.h)
add_executable(fcfs fcfs_test.c scheduler.c scheduler.h fcfs.c fcfs.h rr.c rr.h)
add_executable(rr rr_test.c scheduler.c scheduler.h rr.c rr.h)