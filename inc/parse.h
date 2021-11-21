#ifndef LOGGER_PARSE_H
#define LOGGER_PARSE_H


#include "ConstString.h"
#include "parse_types.h"


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


namespace detail {


/*
 *
 * Functions to determine the size of data structures
 *
 */


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
constexpr unsigned strlen_braces() {
    unsigned result = 0;

    bool brace_open = false;
    for (unsigned i = 0; i < s.size(); ++i) {
        if (!brace_open) {
            if (s[i] == '{') {
                brace_open = true;
                ++result;
            }
        } else {
            ++result;
            if (s[i] == '}') {
                brace_open = false;
            }
        }
    }

    return result;
}

template <ConstString s>
constexpr int get_ast_len() {
    return (s.size() - strlen_braces<s>() + count_braces<s>());
}


/*
 *
 * Parsing functions
 *
 */


template <std::size_t N>
constexpr bool is_digit(ConstString<N> s, unsigned i) {
    return (s[i] > 47) && (s[i] < 58);
}


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
constexpr parse_result_t<fmt_node_t> parse_fmt_string(unsigned i) {
    fmt_node_t result;

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
constexpr parse_result_t<fmt_node_t> parse_braces(unsigned i) {
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
constexpr parse_result_t<string_result_t<get_ast_len<s>()>> parse_string() {
    parse_result_t<string_result_t<get_ast_len<s>()>> result;
    result.is_valid = true;

    unsigned ast_position = 0;

    unsigned i = 0;
    while (i < s.size()) {
        if (s[i] == '{') {
            ++i;

            auto [is_valid, new_i, format_node] = parse_braces<s>(i);
            if (!is_valid) {
                return {false, i, {}};
            }
            i = new_i;
            result.value[ast_position++].set_node(format_node);

        } else if (s[i] == '}') {
            return {false, i, {}};
        } else {
            result.value[ast_position++].set_char(s[i]);
            ++i;
        }
    }

    return result;
}


} // namespace detail



#endif // LOGGER_PARSE_H
