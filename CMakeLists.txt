cmake_minimum_required(VERSION 3.16)

project(main)

add_executable(main CPUtime.cpp)
add_executable(main Radix_my_love.cpp)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)
