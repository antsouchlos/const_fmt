cmake_minimum_required(VERSION 3.21)
project(logger)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


add_executable(logger src/main.cpp)


if(MSVC)
    target_compile_options(logger PRIVATE /W4 /WX)
else()
    target_compile_options(logger PRIVATE -Wall -Wextra -pedantic)
endif()


include_directories(inc)