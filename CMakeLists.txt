cmake_minimum_required(VERSION 3.25)

project(task_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(task_scheduler
    include/thread_pool.h
    src/thread_pool.cpp
    include/concurrent_queue.h
)

target_include_directories(task_scheduler
    PUBLIC
        include
)

add_executable(ex_adding_one
    examples/main.cpp
)

target_link_libraries(ex_adding_one
    PRIVATE
        task_scheduler
)
