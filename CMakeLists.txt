cmake_minimum_required(VERSION 3.25)

project(task_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(task_scheduler
    include/thread_pool.h
    src/thread_pool.cpp
    include/concurrent_queue.h
    include/visualizer.h
    src/visualizer.cpp
)

target_include_directories(task_scheduler
    PUBLIC
        include
)

add_subdirectory(examples/simple/)
add_subdirectory(examples/monte_carlo_pi/)
add_subdirectory(examples/matrix_multiplication/)

