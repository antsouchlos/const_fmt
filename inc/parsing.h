#ifndef LOGGER_PARSING_H
#define LOGGER_PARSING_H


#include <utility.h>


namespace detail {


/*
 *
 * Parse format string
 *
 */


enum class FormatType { s, c, b, B, d, o, x, X, a, A, e, E, f, F, g, G, p };

// TODO: Maybe remove the length from here
template <typename result_t>
struct parse_result_t {
    bool     is_valid  = false;
    unsigned new_index = 0;
    result_t result;
};

struct fmt_string_result_t {
    bool       has_zero_padding = false;
    int        length           = 6;
    int        precision        = 2;
    FormatType type             = FormatType::s;
};

template <std::size_t N>
using string_result_t = std::array<fmt_string_result_t, N>;


// clang-format off

/*
 *
 *  fmtlib grammar:
 *      replacement_field ::=  "{" [arg_id] [":" (format_spec | chrono_format_spec)] "}"
 *      arg_id            ::=  integer | identifier
 *      integer           ::=  digit+
 *      digit             ::=  "0"..."9"
 *      identifier        ::=  id_start id_continue*
 *      id_start          ::=  "a"..."z" | "A"..."Z" | "_"
 *      id_continue       ::=  id_start | digit
 *
 *      format_spec ::=  [[fill]align][sign]["#"]["0"][width]["." precision]["L"][type]
 *      fill        ::=  <a character other than '{' or '}'>
 *      align       ::=  "<" | ">" | "^"
 *      sign        ::=  "+" | "-" | " "
 *      width       ::=  integer | "{" [arg_id] "}"
 *      precision   ::=  integer | "{" [arg_id] "}"
 *      type        ::=  "a" | "A" | "b" | "B" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" |
 *                       "o" | "p" | "s" | "x" | "X"
 *
 *  grammar:
 *      string     ::= { braces | <a character other than '{' or '}'> }
 *      braces     ::= "{" [":" fmt_string] "}"
 *      fmt_string ::= ["0"][width]["." precision][type]
 *
 */

// clang-format on


template <std::size_t N>
constexpr bool is_digit(ConstString<N> s, unsigned i) {
    return (s[i] > 47) && (s[i] < 58);
}


/*
 *
 * Functions to convert X to char array
 * and ConstString to X
 *
 */


template <ConstString s>
constexpr parse_result_t<int> parse_number(unsigned i) {
    int number = 0;

    while ((i < s.size()) && is_digit(s, i)) {
        number = number * 10;
        number += (s[i] - 48);
        ++i;
    }

    return {true, i, number};
}

template <ConstString s>
constexpr parse_result_t<FormatType> parse_type(unsigned i) {
    if (s[i] == 's') { // string
        ++i;
        return {true, i, FormatType::s};
    } else if (s[i] == 'c') { // char
        ++i;
        return {true, i, FormatType::c};
    } else if (s[i] == 'b') { // int
        ++i;
        return {true, i, FormatType::b};
    } else if (s[i] == 'B') {
        ++i;
        return {true, i, FormatType::B};
        //    } else if (s[i] == 'c') {
        //        ++i;
        //        return {true, i, FormatType::c};
    } else if (s[i] == 'd') {
        ++i;
        return {true, i, FormatType::d};
    } else if (s[i] == 'o') {
        ++i;
        return {true, i, FormatType::o};
    } else if (s[i] == 'x') {
        ++i;
        return {true, i, FormatType::x};
    } else if (s[i] == 'X') {
        ++i;
        return {true, i, FormatType::X};
    } else if (s[i] == 'a') { // float
        ++i;
        return {true, i, FormatType::a};
    } else if (s[i] == 'A') {
        ++i;
        return {true, i, FormatType::A};
    } else if (s[i] == 'e') {
        ++i;
        return {true, i, FormatType::e};
    } else if (s[i] == 'E') {
        ++i;
        return {true, i, FormatType::E};
    } else if (s[i] == 'f') {
        ++i;
        return {true, i, FormatType::f};
    } else if (s[i] == 'F') {
        ++i;
        return {true, i, FormatType::F};
    } else if (s[i] == 'g') {
        ++i;
        return {true, i, FormatType::g};
    } else if (s[i] == 'G') {
        ++i;
        return {true, i, FormatType::G};
    } else if (s[i] == 'p') { // pointer
        ++i;
        return {true, i, FormatType::p};
    }

    return {false, i, FormatType::s};
}

template <ConstString s>
constexpr parse_result_t<fmt_string_result_t> parse_fmt_string(unsigned i) {

    fmt_string_result_t result;

    if (s[i] == '0') {
        ++i;
        result.has_zero_padding = true;
    }

    if (is_digit(s, i)) {
        auto [is_valid, new_i, number] = parse_number<s>(i);
        if (!is_valid)
            return {false, i, result};
        i             = new_i;
        result.length = number;
    }

    if (s[i] == '.') {
        ++i;
        auto [is_valid, new_i, number] = parse_number<s>(i);
        if (!is_valid)
            return {false, i, result};
        i                = new_i;
        result.precision = number;
    }

    if (s[i] != '}') {
        auto [is_valid, new_i, type] = parse_type<s>(i);
        if (!is_valid)
            return {false, i, result};
        i           = new_i;
        result.type = type;
    }

    return {true, i, result};
}

template <ConstString s>
constexpr parse_result_t<fmt_string_result_t> parse_braces(unsigned i) {
    if (s[i] == '}') {
        ++i;
        return {true, i, {}};
    } else if (s[i] == ':') {
        ++i;

        auto [is_valid, new_i, format_node] = parse_fmt_string<s>(i);
        if (!is_valid)
            return {false, i, {}};
        i = new_i;

        if (s[i] == '}') {
            ++i;
            return {true, i, format_node};
        }
    }

    return {false, i, {}};
}

template <ConstString s>
constexpr unsigned count_braces() {
    unsigned result = 0;

    for (unsigned i = 0; i < s.size(); ++i) {
        if (s[i] == '{')
            ++result;
    }

    return result;
}

template <ConstString s>
constexpr parse_result_t<string_result_t<count_braces<s>()>> parse_string() {
    parse_result_t<string_result_t<count_braces<s>()>> result;
    result.is_valid = true;

    unsigned format_node_pos = 0;

    for (unsigned i = 0; i < s.size(); ++i) {
        if (s[i] == '{') {
            ++i;

            auto [is_valid, new_i, format_node] = parse_braces<s>(i);
            if (!is_valid) {
                return {false, i, {}};
            }
            i = new_i;
            result.result[format_node_pos] = format_node;

        } else if (s[i] == '}') {
            return {false, i, {}};
        }
    }

    return result;
}

template <ConstString s>
constexpr int get_output_len() {
    constexpr auto result = parse_string<s>();

    // TODO
    return result.is_valid;
}


} // namespace detail


template <detail::ConstString s, typename... args_t>
constexpr std::array<char, detail::get_output_len<s>()> format(args_t...) {
    constexpr int len = detail::get_output_len<s>();
    static_assert(len > 0, "Syntax error in log string");

    std::cout << "Computed Length: " << len << std::endl;

    return {0};
}


#endif // LOGGER_PARSING_H
