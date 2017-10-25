# Syntax for this file: https://cmake.org/cmake/help/v2.8.12/cmake.html
# Reference manual: http://doc.qt.io/qt-5/cmake-manual.html

cmake_minimum_required(VERSION 2.8.12)
# 2.8.11 is required for automatic linking to the qtmain.lib library on Windows

project(RummViz CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc/resource/ui automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets [...])

INCLUDE_DIRECTORIES(include)

QT5_WRAP_UI( UI_HEADERS ui/mainwindow.ui )

# Tell CMake to create the example-project executable
add_executable(RummViz

    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ${UI_HEADERS}
    )

# Use the Widgets module from Qt 5.
target_link_libraries(RummViz Qt5::Widgets)
