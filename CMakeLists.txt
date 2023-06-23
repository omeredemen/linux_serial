cmake_minimum_required(VERSION 3.14.1)
project(serial)

add_executable(main main.cpp src/serial.cpp)
target_include_directories(main PUBLIC include/)