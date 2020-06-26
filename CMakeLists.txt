cmake_minimum_required(VERSION 3.16)

project(main)

set(SOURCE_EXE CPUtime.cpp Radix_my_love.cpp)
add_executable(main main ${SOURCE_EXE})


install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)
