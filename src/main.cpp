#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
};


int main() {
    const auto formatted = format<"Test: {:8.3} {:6.3} {:05.2}">(1432.4334, "abc", 1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return 0;
}