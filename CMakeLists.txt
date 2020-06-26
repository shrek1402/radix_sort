cmake_minimum_required(VERSION 3.16)

project(main)

set(SOURCE CPUtime.cpp Radix_my_love.cpp)
add_executable(main ${SOURCE})

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)
