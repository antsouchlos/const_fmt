#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
};


int main() {
    const auto formatted = format<"Test: {:3.8f} {:5.5} {:05.2}">(3.4, "abc", 1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}