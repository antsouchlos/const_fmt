#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
};


int main() {
    constexpr auto formatted = format<"Test: {:012.5} {} {:034}">(142.4334, "abc", 1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}