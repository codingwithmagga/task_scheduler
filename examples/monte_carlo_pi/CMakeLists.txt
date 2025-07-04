cmake_minimum_required(VERSION 3.25)

project(monte_carlo_pi LANGUAGES CXX)

add_executable(monte_carlo_pi
    main.cpp
)

target_link_libraries(monte_carlo_pi
    PRIVATE
        task_scheduler
)
