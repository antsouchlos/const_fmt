#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
};


int main() {
    constexpr detail::ConstString s{"Test: {:3.8f} {:02.5} {:05.2}"};

    const auto formatted = format<s>(3.4, "abc", 8.98754);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}