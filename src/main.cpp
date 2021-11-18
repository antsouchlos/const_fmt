#include <iostream>

#include <Logger.h>


class Uart {
public:
    void write(char c) {
        std::cout << c;
    }
private:
};


int main() {
    Uart uart;

    Logger logger(uart);
    logger.log<"Test:{:88}">(1, 2, 3);

    return 0;
}