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
    logger.log<"Test:{}">(1, 2, 3);

    std::cout << std::endl;

    constexpr auto ast = detail::parse_string<"Test: {:16.8} {:03.5} {:08.2}">();
    static_assert(ast.is_valid);

    for (const auto& format_node : ast.result) {
        std::cout << "\tFormat Node:" << std::endl;
        std::cout << "\t\thas_zero_padding:\t" << format_node.has_zero_padding << std::endl;
        std::cout << "\t\tlength:\t\t\t\t" << format_node.length << std::endl;
        std::cout << "\t\tprecision:\t\t\t" << format_node.precision << std::endl;
        std::cout << "\t\ttype:\t\t\t\t" << static_cast<int>(format_node.type) << std::endl;
    }

/*
    constexpr detail::ConstString s = "{:8.14c}";

    constexpr auto result = detail::parse_fmt_string<s>(2);

    std::cout << "Is valid: " << result.is_valid << std::endl << std::endl;

    std::cout << "\tresult.has_zero_padding: " << result.result.has_zero_padding << std::endl;
    std::cout << "\tresult.length: " << result.result.length << std::endl;
    std::cout << "\tresult.precision: " << result.result.precision << std::endl;
    std::cout << "\tresult.type: " << static_cast<int>(result.result.type) << std::endl;
*/

    return 0;
}