#include <iostream>

#include <Logger.h>


int main() {
    constexpr auto formatted = "Test: {:012.5} {:6} {:8}"_const_fmt(142.4334, "abcdef", -1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}