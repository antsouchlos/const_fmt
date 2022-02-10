# const_fmt

An extremely lightweight library, intentionally resembling `fmtlib` as much as possible. This is achieved by moving
as much of the formatting process as possible to compile time.

Meant for systems with very few resources, such as embedded systems.

## Overview

During compile-time, the string to be formatted is preprocessed to the point only the actual values to be formatted
have to be written (If they are not available at compile time).

For example `One number: {:03}; And another one: {:05.3}` is preprocessed into `One number: 000; And another one: 00.000`.
This is returned as a `std::array<char, N>`, where `N` is automatically evaluated. The only code executed at compile
time then formats the numbers and writes them into their place in the array.

Disclaimer: The actual formatting code is largely shamelessly stolen from `fmtlib`.

## Including in a project

In order to keep it as lightweight and optimizable as possible, `const_fmt` is implemented as a header-only
library.

This means that using it in a project is as simple as cloning this repo (or e.g. adding it as a submodule)
and adding the repository root folder to the compiler include directories.

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

## Limitations

Only a relatively limited subset of the `fmtlib` syntax is recognized (for now anyway). In particular,
there is no support for positional arguments, alignment, chrono format specs and custom format specifications.

By nature of the library design, which forces compile-time preprocessing of the format string, no dynamic width or
dynamic precision can be implemented.
