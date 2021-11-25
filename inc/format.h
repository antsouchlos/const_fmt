#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include "parse.h"
#include "utility.h"
#include <cstring>


namespace detail {


/*
 *
 * Utility functions
 *
 */


template <ConstString s>
constexpr int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in format string");

    return get_ast_output_len<parse_result.value>();
}

template <fmt_node_t fmt_node, typename T>
constexpr void check_fmt_params() {
    static_assert(fmt_node.length > fmt_node.precision + 1,
                  "Insufficient length for desired precision");
}


/*
 *
 * Type-specific formatting functions
 *      (Most of this code is shamelessly stolen from fmtlib)
 */


// TODO


/*
 *
 *  Formatting wrapper functions
 *
 */


template <std::integral arg_t>
constexpr void format_arg(char* dest, fmt_data_t fmt_data, arg_t) {
    *(dest + fmt_data.position) = 'i';
    //    dest = dest + fmt_data.position;
    //    format_int(dest, fmt_data.length, arg);
};
template <std::floating_point arg_t>
constexpr void format_arg(char* dest, fmt_data_t fmt_data, arg_t) {
    dest    = dest + fmt_data.position;
    *(dest) = 'f';
    *(dest + fmt_data.length - fmt_data.precision - 1) = '.';
};
// TODO: Error handling
constexpr void format_arg(char* dest, fmt_data_t fmt_data, const char* arg) {
    const std::size_t len = const_strlen(arg);
    if (len > fmt_data.length) return;

    dest = dest + fmt_data.position + fmt_data.length - len;

    if (!std::is_constant_evaluated()) {
        std::memcpy(dest, arg, len);
        return;
    }
    for (std::size_t i = 0; i < len; ++i) {
        *(dest++) = *(arg++);
    }
};


template <auto ast>
constexpr void format_args(char*) {
}

template <auto fmt_data_array, typename first_arg_t, typename... args_t>
constexpr void format_args(char* dest, first_arg_t first_arg, args_t... args) {
    format_arg(dest, fmt_data_array[0], first_arg);
    format_args<drop_first(fmt_data_array)>(dest, args...);
}


template <auto ast>
consteval std::array<char, get_ast_output_len<ast>()> get_preproc_string() {
    auto result = get_zero_array<get_ast_output_len<ast>()>();

    int i = 0;

    for (const auto& ast_node : ast) {
        if (ast_node.is_char())
            result[i++] = ast_node.get_char();
        else
            i += ast_node.get_node().length;
    }

    return result;
}


} // namespace detail


/*
 *
 * Public API
 *
 */


template <detail::ConstString s, typename... args_t>
constexpr std::array<char, detail::get_output_len<s>()> format(args_t... args) {
    constexpr auto ast      = detail::parse_string<s>().value;
    constexpr auto fmt_data = detail::get_fmt_data<ast>();

    auto result = detail::get_preproc_string<ast>();

    detail::format_args<fmt_data>(result.begin(), args...);

    return result;
}


#endif // LOGGER_FORMAT_H
