cmake_minimum_required(VERSION 3.25)

project(matrix_multiplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix_multiplication
    main.cpp
)

target_link_libraries(matrix_multiplication
    PRIVATE
        task_scheduler
)
