# const_fmt

An extremely lightweight library, intentionally resembling `fmtlib` as much as possible. This is achieved by moving
as much of the formatting process as possible to compile time.

Meant for systems with very few resources, such as embedded systems.

## Overview

During compile-time, the string to be formatted is preprocessed to the point, only the actual values to be formatted
have to be written (If they are not available at compile time).

For example `One number: {:08}; And another one: {:05.3}` is preprocessed into `One number: 000; And another one: 00.000`.
This is returned as a `std::array<char, N>`, where `N` is automatically evaluated. The only code executed at compile
time then formats the numbers and writes them into their place in the array.

Disclaimer: The actual formatting code is largely shamelessly stolen from `fmtlib`.

## Building and running the tests

1. Initialize the `googletest` framework submodule
```bash
$ git submodule update --init
```

2. Create the build directory
```bash
$ cmake -B build -S .
```

3. Build the project
```bash
$ cmake --build build/
```

4. Run the tests
```bash
$ ctest --test-dir build/
```