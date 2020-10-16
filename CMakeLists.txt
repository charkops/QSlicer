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
        Writer.cpp
)

add_library(qslicer ${lib_source_files})

add_executable(main main.cpp)
target_link_libraries(main qslicer)

# Visualization module
find_package(PCL REQUIRED COMPONENETS common visualization)
if (PCL_FOUND)
  message(STATUS "Found PCL Version: ${PCL_VERSION}")

  # Build visualizer
  add_executable(visualizer visualizer.cpp)  
  target_include_directories(visualizer PRIVATE ${PCL_INCLUDE_DIRS})
  target_link_libraries(visualizer qslicer ${PCL_LIBRARIES})

  add_executable(visualizer_support visualizer_support.cpp)  
  target_include_directories(visualizer_support PRIVATE ${PCL_INCLUDE_DIRS})
  target_link_libraries(visualizer_support qslicer ${PCL_LIBRARIES})
endif (PCL_FOUND)