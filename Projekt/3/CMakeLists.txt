cmake_minimum_required(VERSION 3.13)
project(Projekt3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projekt3 main.cpp Client.cpp Client.h Room.cpp Room.h Resources.cpp Resources.h)
target_link_libraries(Projekt3 pthread curses)