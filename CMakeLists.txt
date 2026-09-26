cmake_minimum_required(VERSION 3.16)
project(tetris)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)

add_executable(tetris main.cpp)

target_include_directories(tetris PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(tetris PRIVATE ${CURSES_LIBRARIES})
