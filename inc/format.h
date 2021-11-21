#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include "parse.h"


namespace detail {


template <ConstString s>
constexpr int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in format string");

    unsigned result = 0;

    for (const auto& ast_node : parse_result.value) {
        if (ast_node.is_char())
            ++result;
        else
            result += ast_node.get_node().length;
    }

    return result;
}


template <fmt_node_t fmt_node, std::integral arg_t>
constexpr std::array<char, fmt_node.length> format_arg(arg_t arg) {
    std::array<char, fmt_node.length> result;

    // TODO: See if this is possible with charconv
    for (unsigned i = 1; i <= result.size(); ++i) {
        result[result.size() - i] = arg % 10 + 48;
        arg = arg / 10;
    }

    return result;
}

template <fmt_node_t fmt_node, std::floating_point arg_t>
constexpr std::array<char, fmt_node.length> format_arg(arg_t) {
    std::array<char, fmt_node.length> result;

    // TODO: See if this is possible with charconv
    for (auto& c : result)
        c = 'f';

    return result;
}

template <fmt_node_t fmt_node>
constexpr std::array<char, fmt_node.length> format_arg(const char* arg) {
    std::array<char, fmt_node.length> result;

    for (auto& c : result) {
        if (*arg != '\0')
            c = *(arg++);
        else
            c = '-';
    }

    return result;
}

template <auto t_ast, unsigned t_ast_i = 0, unsigned t_result_i = 0,
        typename char_array_t>
constexpr char_array_t format_args(char_array_t result) {
    return result;
}



template <auto t_ast, unsigned t_ast_i = 0, unsigned t_result_i = 0,
          typename char_array_t, typename first_arg_t, typename... other_args_t>
constexpr char_array_t format_args(char_array_t result, first_arg_t first_arg,
                                   other_args_t... other_args) {
    if constexpr (t_ast_i >= t_ast.size()) {
        return result;
    } else {
        constexpr auto ast_node = t_ast[t_ast_i];

        if (ast_node.is_char()) {
            result[t_result_i] = ast_node.get_char();
            return format_args<t_ast, t_ast_i + 1, t_result_i + 1>(
                result, first_arg, other_args...);
        } else {
            constexpr auto fmt_node      = ast_node.get_node();
            const auto     formatted_arg = format_arg<fmt_node>(first_arg);

            std::copy(formatted_arg.begin(), formatted_arg.end(),
                      result.begin() + t_result_i);

            return format_args<t_ast, t_ast_i + 1,
                               t_result_i + fmt_node.length>(result, other_args...);
        }
    }
}


} // namespace detail


template <detail::ConstString s, typename... args_t>
constexpr std::array<char, detail::get_output_len<s>()> format(args_t... args) {
    constexpr auto parse_result = detail::parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in format string");

    std::array<char, detail::get_output_len<s>()> result;

    return detail::format_args<parse_result.value>(result, args...);
}


#endif // LOGGER_FORMAT_H
