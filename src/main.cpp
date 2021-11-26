//#include <iostream>

#include <Logger.h>


int main(int argc, char* argv[]) {
    auto formatted = "Test: {:12} {:012.5} {:8}"_fmt(argv[0], argc, -1234);

    for (const auto& c : formatted)
        std::cout << c;
    std::cout << std::endl;

    return formatted[6];
}