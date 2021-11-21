#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
};


int main() {
    constexpr detail::ConstString s{"Test: {:16.8f} {:03.5} {:08.2}"};

    const auto formatted = format<s>(3.4, "abc", 8.98754);

    return 0;
}