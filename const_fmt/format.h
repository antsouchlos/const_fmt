#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include <cstring>

#include "format_impl.h"
#include "parse.h"
#include "utility.h"


namespace const_fmt { namespace const_fmt_detail {


/*
 *
 * Utility functions
 *
 */


template <ConstString s>
constexpr inline int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in const_format string");

    return get_ast_output_len<parse_result.value>();
}

template <fmt_node_t fmt_node, typename T>
constexpr inline void check_fmt_params() {
    static_assert(fmt_node.length > fmt_node.precision + 1,
                  "Insufficient length for desired precision");
}


/*
 *
 *  Formatting wrapper functions
 *
 */

template <fmt_data_t fmt_data, std::integral arg_t>
constexpr inline void format_arg(char* dest, arg_t arg) {
    const_fmt_detail::format_int<arg_t, fmt_data>(dest, arg);
};

template <fmt_data_t fmt_data, std::floating_point arg_t>
constexpr inline void format_arg(char* dest, arg_t arg){
    const_fmt_detail::format_float<arg_t, fmt_data>(dest, arg);
};

// TODO: Error handling
template <fmt_data_t fmt_data>
constexpr inline void format_arg(char* dest, const char* arg) {
    const std::size_t len = const_strlen(arg);
    if (len > fmt_data.length) return;

    dest = dest + fmt_data.length - len;

    if (!std::is_constant_evaluated()) {
        std::memcpy(dest, arg, len);
        return;
    }
    for (std::size_t i = 0; i < len; ++i) {
        *(dest++) = *(arg++);
    }
};


// End of recursion
template <auto ast>
constexpr inline void format_args(char*) {
}

template <auto fmt_data_array, typename first_arg_t, typename... args_t>
constexpr inline void format_args(char* dest, first_arg_t first_arg,
                                  args_t... args) {
    format_arg<fmt_data_array[0]>(dest + fmt_data_array[0].position, first_arg);
    format_args<drop_first(fmt_data_array)>(dest, args...);
}


template <auto ast>
consteval inline auto get_preproc_string() {
    std::array<char, get_ast_output_len<ast>()> result;

    int i = 0;

    for (const auto& ast_node : ast) {
        if (ast_node.is_char()) {
            result[i++] = ast_node.get_char();
        } else {
            for (int j = 0; j < ast_node.get_node().length; ++j)
                result[i++] = ast_node.get_node().has_zero_padding ? '0' : ' ';
        }
    }

    return result;
}


}} // namespace const_fmt::const_fmt_detail


/*
 *
 * Public Interface
 *
 */


namespace const_fmt {


template <const_fmt_detail::ConstString s, typename... args_t>
constexpr inline auto const_format(args_t... args) {
    constexpr auto ast      = const_fmt_detail::parse_string<s>();
    constexpr auto fmt_data = const_fmt_detail::get_fmt_data<ast.value>();

    static_assert(ast.is_valid, "Invalid format string");

    auto result = const_fmt_detail::get_preproc_string<ast.value>();

    const_fmt_detail::format_args<fmt_data>(result.begin(), args...);

    return result;
}


template <const_fmt_detail::ConstString t_s>
class const_fmt_literal_obj_t {
public:
    template <typename... args_t>
    constexpr auto operator()(args_t... args) {
        return const_format<t_s>(args...);
    }
};

template <const_fmt_detail::ConstString t_s>
constexpr auto operator""_const_fmt() {
    return const_fmt_literal_obj_t<t_s>{};
}


} // namespace const_fmt


#endif // LOGGER_FORMAT_H
