#ifndef LOGGER_UTILITY_H
#define LOGGER_UTILITY_H


#include <cstdlib>


namespace detail {


constexpr std::size_t const_pow(std::size_t base, std::size_t pow) {
    if (pow == 0)
        return 1;
    else
        return base * const_pow(base, pow - 1);
}


template <fmt_node_t fmt_node>
constexpr std::array<char, fmt_node.length> get_init_array() {
    std::array<char, fmt_node.length> result;

    if constexpr (fmt_node.has_zero_padding) {
        for (auto& c : result)
            c = '0';
    } else {
        for (auto& c : result)
            c = ' ';
    }

    return result;
}


} // namespace detail


#endif // LOGGER_UTILITY_H
