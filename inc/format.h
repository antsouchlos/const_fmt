#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H


#include "parse.h"
#include "utility.h"


namespace detail {

template <fmt_node_t fmt_node, typename T>
constexpr void check_fmt_params() {
    static_assert(fmt_node.length > fmt_node.precision + 1,
                  "Insufficient length for desired precision");
}


template <auto t_ast>
consteval int get_ast_output_len() {
    unsigned result = 0;

    for (const auto& ast_node : t_ast) {
        if (ast_node.is_char())
            ++result;
        else
            result += ast_node.get_node().length;
    }

    return result;
}

template <ConstString s>
constexpr int get_output_len() {
    constexpr auto parse_result = parse_string<s>();
    static_assert(parse_result.is_valid, "Syntax error in format string");

    return get_ast_output_len<parse_result.value>();
}

//// TODO: See if this is possible with <charconv>
//// TODO: Steal some code from fmtlib
//// TODO: In case of error, set chars to all 'f's
// template <fmt_node_t fmt_node, std::integral arg_t>
// constexpr std::array<char, fmt_node.length> format_arg(arg_t arg) {
//     check_fmt_params<fmt_node, arg_t>();
//
//     auto result = get_init_array<fmt_node>();
//
//     unsigned offset = 0;
//
//     if (arg < 0) {
//         result[0] = '-';
//         arg       = -arg;
//         ++offset;
//     }
//
//     for (int i = result.size() - 1;
//          (i >= static_cast<int>(offset)) && (arg > 0); --i) {
//         result[i] = arg % 10 + 48;
//         arg       = arg / 10;
//     }
//
//     return result;
// }
//
//// TODO: See if this is possible with <charconv>
//// TODO: Steal some code from fmtlib
//// TODO: In case of error, set chars to all 'f's
// template <fmt_node_t fmt_node, std::floating_point arg_t>
// constexpr std::array<char, fmt_node.length> format_arg(arg_t arg) {
//     check_fmt_params<fmt_node, arg_t>();
//
//     constexpr unsigned point_index = fmt_node.length - fmt_node.precision -
//     1; constexpr unsigned multiplier  = const_pow(10, fmt_node.precision);
//
//     auto result         = get_init_array<fmt_node>();
//     result[point_index] = '.';
//
//
//     arg = arg * multiplier;
//
//     for (int i = result.size() - 1;
//          (i > static_cast<int>(point_index)) && (arg >= 1); --i) {
//
//         result[i] = static_cast<int>(arg) % 10 + 48;
//         arg       = arg / 10;
//     }
//
//     for (int i = point_index - 1; (i >= 0) && (arg >= 1); --i) {
//         result[i] = static_cast<int>(arg) % 10 + 48;
//         arg       = arg / 10;
//     }
//
//     return result;
// }
//
//// TODO: Steal some code from fmtlib
//// TODO: In case of error, set chars to all 'f's
// template <fmt_node_t fmt_node>
// constexpr std::array<char, fmt_node.length> format_arg(const char* arg) {
//     check_fmt_params<fmt_node, const char*>();
//
//     std::array<char, fmt_node.length> result;
//
//     for (auto& c : result) {
//         if (*arg != '\0')
//             c = *(arg++);
//         else
//             c = ' ';
//     }
//
//     return result;
// }
//
//
// template <auto t_ast, unsigned t_ast_i = 0, unsigned t_result_i = 0,
//           typename char_array_t>
// constexpr char_array_t format_args(char_array_t result) {
//     return result;
// }

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


template <std::integral arg_t>
constexpr void format_arg(char* dest, fmt_data_t fmt_data, arg_t arg) {
    *(dest + fmt_data.position) = 'i';
//    dest = dest + fmt_data.position;
//    format_int(dest, fmt_data.length, arg);
};
template <std::floating_point arg_t>
constexpr void format_arg(char* dest, fmt_data_t fmt_data, arg_t) {
    *(dest + fmt_data.position) = 'f';
};
constexpr void format_arg(char* dest, fmt_data_t fmt_data, const char*) {
    *(dest + fmt_data.position) = 'c';
};


template <auto ast>
constexpr void format_args(char*) {
}

template <auto fmt_data_array, typename first_arg_t, typename... args_t>
constexpr void format_args(char* dest, first_arg_t first_arg, args_t... args) {
    format_arg(dest, fmt_data_array[0], first_arg);
    format_args<drop_first(fmt_data_array)>(dest, args...);
}


} // namespace detail


template <detail::ConstString s, typename... args_t>
constexpr std::array<char, detail::get_output_len<s>()> format(args_t... args) {
    constexpr auto ast      = detail::parse_string<s>().value;
    constexpr auto fmt_data = detail::get_fmt_data<ast>();

    auto result = detail::get_preproc_string<ast>();

    detail::format_args<fmt_data>(result.begin(), args...);

    return result;
}


#endif // LOGGER_FORMAT_H
