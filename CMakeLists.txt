cmake_minimum_required( VERSION 3.0)

project(sendIR-RPi C)

set(CMAKE_C_FLAGS "-Wall")

add_executable(sendIR-RPi
        src/sendIR.c)

add_executable(sendIR2-RPi
		src/sendIR2.c)

include_directories(CMAKE_SOURCE_DIR/include)

find_library(wiringPi_LIB wiringPi)
target_link_libraries(sendIR-RPi ${wiringPi_LIB})
target_link_libraries(sendIR2-RPi ${wiringPi_LIB})
