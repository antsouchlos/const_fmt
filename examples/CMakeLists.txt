add_executable(const_fmt_example src/examples.cpp)

if(MSVC)
    target_compile_options(const_fmt_example PRIVATE /W4 /WX)
else()
    target_compile_options(const_fmt_example PRIVATE -O3 -Wall -Wextra -pedantic -fno-exceptions)
endif()