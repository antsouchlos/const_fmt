cmake_minimum_required(VERSION 3.21)
project(logger)

set(CMAKE_CXX_STANDARD 20)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


include_directories(inc)

add_executable(logger src/main.cpp)


if(MSVC)
    target_compile_options(logger PRIVATE /W4 /WX)
else()
    target_compile_options(logger PRIVATE -O3 -Wall -Wextra -pedantic -fno-exceptions)
endif()


option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()
