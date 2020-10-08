cmake_minimum_required(VERSION 3.10)

project(QSlicer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (lib_source_files 
        Point.cpp
        Line.cpp
        Triangle.cpp
        io.cpp
        Slice.cpp
)

add_library(qslicer ${lib_source_files})

add_executable(main main.cpp)
target_link_libraries(main qslicer)