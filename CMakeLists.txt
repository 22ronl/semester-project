cmake_minimum_required(VERSION 3.12.2)
project(semsterProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(semsterProject main.cpp commands.cpp  expression.cpp
                maps.cpp parser.cpp  utils.cpp command_expression.h plane_data.h searchable.cpp searcher.cpp searcher.h)

set(CMAKE_CXX_FLAGS -pthread)