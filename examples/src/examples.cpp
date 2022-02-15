#include <const_fmt/format.h>
#include <iostream>
#include <string_view>

using namespace const_fmt;

int main() {
    constexpr auto s = "This is an integer: {:08x}, and this is a float: {:09.4b}"_const_fmt(125u, -86.2);

    // Convert s (with a type of 'std::array<char, N>') into something
    // writable to std::cout
    std::string_view sv{&s[0], s.size()};

    std::cout << sv << std::endl;

    return 0;
}