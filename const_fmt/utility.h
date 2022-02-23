#ifndef LOGGER_UTILITY_H
#define LOGGER_UTILITY_H


#include <string.h>

#include "stdlib.h"
#include "types.h"


namespace const_fmt { namespace const_fmt_detail {


constexpr inline std::size_t const_pow(std::size_t base, std::size_t pow) {
    if (pow == 0)
        return 1;
    else
        return base * const_pow(base, pow - 1);
}


template <auto ast>
consteval inline std::size_t count_ast_format_nodes() {
    std::size_t result = 0;

    for (const auto& node : ast)
        if (!node.is_char()) ++result;

    return result;
}


template <auto ast>
consteval inline std::array<fmt_data_t, count_ast_format_nodes<ast>()>
get_fmt_data() {
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
consteval inline std::array<elem_t, t_n - 1>
drop_first(std::array<elem_t, t_n> array) {
    static_assert(t_n > 0,
                  "Can't drop first element of array with no elements");

    std::array<elem_t, t_n - 1> result;

    std::copy(array.begin() + 1, array.end(), result.begin());

    return result;
}


template <auto t_ast>
consteval inline int get_ast_output_len() {
    unsigned result = 0;

    for (const auto& ast_node : t_ast) {
        if (ast_node.is_char())
            ++result;
        else
            result += ast_node.get_node().length;
    }

    return result;
}

constexpr inline std::size_t const_strlen(const char* arg) {
    if (std::is_constant_evaluated()) {
        return *arg ? 1 + const_strlen(arg + 1) : 0;
    } else {
        return strlen(arg);
    }
}


}} // namespace const_fmt::const_fmt_detail


#endif // LOGGER_UTILITY_H
