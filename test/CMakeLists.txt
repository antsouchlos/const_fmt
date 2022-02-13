cmake_minimum_required(VERSION 3.21)
project(logger)


set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


include_directories(..)


enable_testing()
include(GoogleTest)


add_subdirectory("googletest")

mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    gtest_discover_tests(${TESTNAME}
            WORKING_DIRECTORY ${PROJECT_DIR}
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
            )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)

    if(MSVC)
        target_compile_options(${TESTNAME} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TESTNAME} PRIVATE -O3 -Wall -Wextra -pedantic -fno-exceptions)
    endif()
endmacro()


package_add_test(utility_test src/utility.cpp)
package_add_test(parse_test src/parse.cpp)
package_add_test(format_test src/format.cpp)

