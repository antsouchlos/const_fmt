#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include <cstring>

#include "parse.h"
#include "utility.h"
#include "format_impl.h"


namespace detail {


/*
 *
 * Utility functions
 *
 */


template <ConstString s>
constexpr inline int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in format string");

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

// TODO: Error handling
template <fmt_data_t fmt_data, std::integral arg_t>
constexpr inline void format_arg(char* dest, arg_t arg) {
    auto error_array = get_init_array<fmt_data.length>('f');

    detail::format_integral(dest, arg, fmt_data);
};
// TODO: Error handling
template <fmt_data_t fmt_data, std::floating_point arg_t>
constexpr inline void format_arg(char* dest, arg_t) {
    *(dest) = 'f';
    *(dest + fmt_data.length - fmt_data.precision - 1) = '.';
};
// TODO: Error handling
template<fmt_data_t fmt_data>
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
constexpr inline void format_args(char* dest, first_arg_t first_arg, args_t... args) {
    format_arg<fmt_data_array[0]>(dest + fmt_data_array[0].position, first_arg);
    format_args<drop_first(fmt_data_array)>(dest, args...);
}


template <auto ast>
consteval inline std::array<char, get_ast_output_len<ast>()> get_preproc_string() {
    auto result = get_init_array<get_ast_output_len<ast>()>('0');

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
constexpr inline auto format(args_t... args) {
    constexpr auto ast      = detail::parse_string<s>().value;
    constexpr auto fmt_data = detail::get_fmt_data<ast>();

    auto result = detail::get_preproc_string<ast>();

    detail::format_args<fmt_data>(result.begin(), args...);

    return result;
}


template<detail::ConstString t_s>
class fmt_literal_obj_t {
    public:
        template<typename... args_t>
        constexpr auto operator()(args_t... args) {
            return format<t_s>(args...);
        }
};

template <detail::ConstString t_s>
constexpr auto operator""_fmt() {
    return fmt_literal_obj_t<t_s>{};
}


#endif // LOGGER_FORMAT_H
