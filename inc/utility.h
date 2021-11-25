#ifndef LOGGER_UTILITY_H
#define LOGGER_UTILITY_H


#include "types.h"


namespace detail {


constexpr std::size_t const_pow(std::size_t base, std::size_t pow) {
    if (pow == 0)
        return 1;
    else
        return base * const_pow(base, pow - 1);
}


template <std::size_t t_n>
constexpr std::array<char, t_n> get_zero_array() {
    std::array<char, t_n> result;

    for (auto& c : result)
        c = '0';

    return result;
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


template <auto ast>
consteval std::size_t count_ast_format_nodes() {
    std::size_t result = 0;

    for (const auto& node : ast)
        if (!node.is_char()) ++result;

    return result;
}


template <auto ast>
consteval std::array<fmt_data_t, count_ast_format_nodes<ast>()> get_fmt_data() {
    std::array<fmt_data_t, count_ast_format_nodes<ast>()> result = {};

    std::size_t position = 0;
    std::size_t i        = 0;

    for (const auto& node : ast) {
        if (!node.is_char()) {
            const auto fmt_node = node.get_node();

            result[i].has_zero_padding = fmt_node.has_zero_padding;
            result[i].length           = fmt_node.length;
            result[i].precision        = fmt_node.precision;
            result[i].type             = fmt_node.type;
            result[i].position         = position;

            ++i;
            position += fmt_node.length;
        } else {
            ++position;
        }
    }

    return result;
}


template <typename elem_t, std::size_t t_n>
consteval std::array<elem_t, t_n - 1>
drop_first(std::array<elem_t, t_n> array) {
    static_assert(t_n > 0,
                  "Can't drop first element of array with no elements");

    std::array<elem_t, t_n - 1> result;

    std::copy(array.begin() + 1, array.end(), result.begin());

    return result;
}


} // namespace detail


#endif // LOGGER_UTILITY_H
