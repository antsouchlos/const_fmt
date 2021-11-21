#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include "parse.h"


namespace detail {


template <ConstString s>
constexpr int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in log string");

    unsigned result = 0;

    for (const auto& ast_node : parse_result.value) {
        if (ast_node.is_char())
            ++result;
        else
            result += ast_node.get_node().length;
    }

    return result;
}

template <typename T, fmt_node_t fmt_node>
constexpr bool is_valid_type() {
    // TODO
    return true;
}

template <typename T, fmt_node_t fmt_node>
constexpr std::array<char, fmt_node.length> format_arg(T arg) {
    static_assert(is_valid_type<T, fmt_node>(), "Invalid argument type");

    std::array<char, fmt_node.length> result;

    for (auto& c : result)
        c = 'f';

    return result;
}


} // namespace detail


template <detail::ConstString s, typename... args_t>
std::array<char, detail::get_output_len<s>()> format(args_t... args) {
    constexpr auto parse_result = detail::parse_string<s>();
    static_assert(parse_result.is_valid);

    std::array<char, detail::get_output_len<s>()> result;

    return result;
}


#endif // LOGGER_FORMAT_H
