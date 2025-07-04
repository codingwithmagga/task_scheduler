cmake_minimum_required(VERSION 3.25)

project(simple_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple_example
    main.cpp
)

target_link_libraries(simple_example
    PRIVATE
        task_scheduler
)
