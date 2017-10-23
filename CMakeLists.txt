cmake_minimum_required(VERSION 2.8)

project(Rummikub)

add_definitions( -std=c++11 )

add_executable(${PROJECT_NAME}
    main.cpp
    base_types.h
    )


