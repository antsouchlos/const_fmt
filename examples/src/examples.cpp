#include <const_fmt/format.h>
#include <iostream>
#include <string_view>

using namespace const_fmt;

int main() {
    constexpr auto s = "abcdef {:04}"_const_fmt(123);

    // Convert the s (with a type of 'std::array<char, N>') into something
    // writable to std::cout
    std::string_view sv{&s[0], s.size()};

    std::cout << sv << std::endl;

    return 0;
}