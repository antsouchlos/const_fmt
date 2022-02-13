cmake_minimum_required(VERSION 3.21)
project(const_fmt)

set(CMAKE_CXX_STANDARD 20)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


include_directories(.)


add_subdirectory(examples)


option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()
