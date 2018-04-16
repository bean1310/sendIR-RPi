cmake_minimum_required( VERSION 3.0)

project(sendIR-RPi C)

set(CMAKE_C_FLAGS "-Wall")

add_executable(sendIR
        sendIR.c)

target_link_libraries(wiringPi)