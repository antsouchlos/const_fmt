#include <iostream>

#include <Logger.h>


int main() {
    constexpr auto formatted = format<"Test: {:012.5} {} {:8}">(142.4334, "abcdef", -1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}