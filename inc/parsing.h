#ifndef LOGGER_PARSING_H
#define LOGGER_PARSING_H


#include <utility.h>


namespace detail {


/*
 *
 * Parse format string
 *
 */


template <typename result_t>
struct parse_result {
    bool     valid     = false;
    unsigned new_index = 0;
    unsigned length    = 0;
    result_t result;
};

enum class FormatType { s, c, b, B, d, o, x, X, a, A, e, E, f, F, g, G, p };


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


template <std::size_t N>
constexpr parse_result<int> parse_number(ConstString<N> s, unsigned i) {
    int number = 0;

    while ((i < s.size()) && is_digit(s, i)) {
        number = number * 10;
        number += (s[i] - 48);
        ++i;
    }

    return {true, i, 0, number};
}

template <std::size_t N>
constexpr parse_result<FormatType> parse_type(ConstString<N> s, unsigned i) {
    if (s[i] == 's') { // string
        ++i;
        return {true, i, 1, FormatType::s};
    } else if (s[i] == 'c') { // char
        ++i;
        return {true, i, 1, FormatType::c};
    } else if (s[i] == 'b') { // int
        ++i;
        return {true, i, 1, FormatType::b};
    } else if (s[i] == 'B') {
        ++i;
        return {true, i, 1, FormatType::B};
//    } else if (s[i] == 'c') {
//        ++i;
//        return {true, i, 1, FormatType::c};
    } else if (s[i] == 'd') {
        ++i;
        return {true, i, 1, FormatType::d};
    } else if (s[i] == 'o') {
        ++i;
        return {true, i, 1, FormatType::o};
    } else if (s[i] == 'x') {
        ++i;
        return {true, i, 1, FormatType::x};
    } else if (s[i] == 'X') {
        ++i;
        return {true, i, 1, FormatType::X};
    } else if (s[i] == 'a') { // float
        ++i;
        return {true, i, 1, FormatType::a};
    } else if (s[i] == 'A') {
        ++i;
        return {true, i, 1, FormatType::A};
    } else if (s[i] == 'e') {
        ++i;
        return {true, i, 1, FormatType::e};
    } else if (s[i] == 'E') {
        ++i;
        return {true, i, 1, FormatType::E};
    } else if (s[i] == 'f') {
        ++i;
        return {true, i, 1, FormatType::f};
    } else if (s[i] == 'F') {
        ++i;
        return {true, i, 1, FormatType::F};
    } else if (s[i] == 'g') {
        ++i;
        return {true, i, 1, FormatType::g};
    } else if (s[i] == 'G') {
        ++i;
        return {true, i, 1, FormatType::G};
    } else if (s[i] == 'p') { // pointer
        ++i;
        return {true, i, 1, FormatType::p};
    }

    return {false, i, 0, FormatType::a};
}

template <std::size_t N>
constexpr std::pair<unsigned, int> parse_fmt_string(ConstString<N> s,
                                                    unsigned       i) {
    int result_extra_len = 0;

    if (s[i] == '0')
        ++i;

    if (is_digit(s, i)) {
        auto [is_valid, new_i, len, number] = parse_number(s, i);
        if (!is_valid) return {i, -1};
        i = new_i;
        result_extra_len += number;
    }

    if (s[i] == '.') {
        ++i;
        auto [is_valid, new_i, len, number] = parse_number(s, i);
        if (!is_valid) return {i, -1};
        i = new_i;
        result_extra_len += len;
    }

    if (s[i] != '}') {
        auto [is_valid, new_i, len, type] = parse_type(s, i);
        if (is_valid) return {i, -1};
        i = new_i;
//        result_extra_len += len;
    }

    return {i, result_extra_len};
}

template <std::size_t N>
constexpr std::pair<unsigned, int> parse_braces(ConstString<N> s, unsigned i) {
    int result_extra_len = 0;

    if (s[i] == '}') {
        ++i;
        return {i, result_extra_len};
    } else if (s[i] == ':') {
        ++i;

        auto [new_i, extra_len] = parse_fmt_string(s, i);
        if (extra_len < 0) return {i, -1};
        i = new_i;
        result_extra_len += extra_len;

        if (s[i] == '}') {
            ++i;
            return {i, result_extra_len};
        }
    }

    return {i, -1};
}

template <std::size_t N>
constexpr int get_output_len(ConstString<N> s) {
    int result_extra_len = 0;

    for (unsigned i = 0; i < s.size(); ++i) {
        if (s[i] == '{') {
            ++i;

            auto [new_i, extra_len] = parse_braces(s, i);
            if (extra_len < 0) return -1;
            i = new_i;
            result_extra_len += extra_len;

        } else if (s[i] == '}') {
            return -1;
        }
    }

    return (result_extra_len + s.size());
}


} // namespace detail


template <detail::ConstString s, typename... args_t>
const std::array<char, detail::get_output_len(s)> format(args_t...) {
    constexpr int len = get_output_len(s);
    static_assert(len > 0, "Syntax error in log string");

    std::cout << "Computed Length: " << len << std::endl;

    return {0};
}


#endif // LOGGER_PARSING_H
