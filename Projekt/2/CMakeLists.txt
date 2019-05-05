cmake_minimum_required(VERSION 3.13)
project(Projekt2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projekt2 main.cpp)

target_link_libraries(Projekt2 curses)
target_link_libraries(Projekt2 pthread)