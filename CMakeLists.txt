cmake_minimum_required(VERSION 2.8)

project(Rummikub)

add_definitions( -std=c++11 )

INCLUDE_DIRECTORIES(.)

add_library(Rummikub
    main.cpp
    base_types.cpp
    base_types.h
    gameplay.cpp
    gameplay.h
    )

add_executable(rummikub-test
    tests/basic_tests.cpp
    tests/play_tests.cpp
    )
target_link_libraries( rummikub-test Rummikub)

add_subdirectory(RummViz)

