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
    //Uart uart;

//    Logger logger(uart);
//    logger.log<"Test:{:8.2}">(1, 2, 3);

    constexpr detail::ConstString s = "{:8.14c}";

    constexpr auto result = detail::parse_fmt_string(s, 2);

    std::cout << "Is valid: " << result.is_valid << std::endl << std::endl;

    std::cout << "\tresult.has_zero_padding: " << result.result.has_zero_padding << std::endl;
    std::cout << "\tresult.length: " << result.result.length << std::endl;
    std::cout << "\tresult.precision: " << result.result.precision << std::endl;
    std::cout << "\tresult.type: " << static_cast<int>(result.result.type) << std::endl;


    return 0;
}