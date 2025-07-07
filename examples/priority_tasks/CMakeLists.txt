cmake_minimum_required(VERSION 3.25)

project(priority_tasks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(priority_tasks
    main.cpp
)

target_link_libraries(priority_tasks
    PRIVATE
        task_scheduler
)
