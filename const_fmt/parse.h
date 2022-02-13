#ifndef LOGGER_PARSE_H
#define LOGGER_PARSE_H


#include "types.h"


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


namespace const_fmt { namespace const_fmt_detail {


/*
 *
 * Functions to determine the size of data structures
 *
 */


template <ConstString s>
constexpr inline unsigned count_braces() {
    unsigned result = 0;

    for (unsigned i = 0; i < s.size(); ++i) {
        if (s[i] == '{') ++result;
    }

    return result;
}

template <ConstString s>
constexpr inline unsigned strlen_braces() {
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
constexpr inline int get_ast_len() {
    return (s.size() - strlen_braces<s>() + count_braces<s>());
}


/*
 *
 * Parsing functions
 *
 */


template <ConstString s>
constexpr inline bool is_digit(unsigned i) {
    return (s[i] > 47) && (s[i] < 58);
}


template <ConstString s>
constexpr inline parse_result_t<unsigned> parse_number(unsigned i) {
    unsigned number = 0;

    if (!is_digit<s>(i)) {
        return {false, i, number};
    }

    while ((i < s.size()) && is_digit<s>(i)) {
        number = number * 10;
        number += (s[i] - 48);
        ++i;
    }

    return {true, i, number};
}

template <ConstString s>
constexpr inline parse_result_t<FormatType> parse_type(unsigned i) {
    switch (s[i]) {
    case 's':
        return {true, ++i, FormatType::s};
    case 'c':
        return {true, ++i, FormatType::c};
    case 'b':
        return {true, ++i, FormatType::b};
    case 'B':
        return {true, ++i, FormatType::B};
    case 'd':
        return {true, ++i, FormatType::d};
    case 'o':
        return {true, ++i, FormatType::o};
    case 'x':
        return {true, ++i, FormatType::x};
    case 'X':
        return {true, ++i, FormatType::X};
    case 'a':
        return {true, ++i, FormatType::a};
    case 'A':
        return {true, ++i, FormatType::A};
    case 'e':
        return {true, ++i, FormatType::e};
    case 'E':
        return {true, ++i, FormatType::E};
    case 'f':
        return {true, ++i, FormatType::f};
    case 'F':
        return {true, ++i, FormatType::F};
    case 'g':
        return {true, ++i, FormatType::g};
    case 'G':
        return {true, ++i, FormatType::G};
    case 'p':
        return {true, ++i, FormatType::p};
    default:
        return {false, i, FormatType::s};
    }
}

template <ConstString s>
constexpr inline parse_result_t<fmt_node_t> parse_fmt_string(unsigned i) {
    fmt_node_t result;

    if (s[i] == '0') {
        ++i;
        result.has_zero_padding = true;
    }

    if (is_digit<s>(i)) {
        auto [is_valid, new_i, number] = parse_number<s>(i);
        if (!is_valid) return {false, i, result};
        i             = new_i;
        result.length = number;
    }

    if (s[i] == '.') {
        ++i;
        auto [is_valid, new_i, number] = parse_number<s>(i);
        if (!is_valid) return {false, i, result};
        i                = new_i;
        result.precision = number;
    }

    if (s[i] != '}') {
        auto [is_valid, new_i, type] = parse_type<s>(i);
        if (!is_valid) return {false, i, result};
        i           = new_i;
        result.type = type;
    }

    return {true, i, result};
}

template <ConstString s>
constexpr inline parse_result_t<fmt_node_t> parse_braces(unsigned i) {
    if (s[i] == '}') {
        ++i;
        return {true, i, {}};
    } else if (s[i] == ':') {
        ++i;

        auto [is_valid, new_i, format_node] = parse_fmt_string<s>(i);
        if (!is_valid) return {false, i, {}};
        i = new_i;

        if (s[i] == '}') {
            ++i;
            return {true, i, format_node};
        }
    }

    return {false, i, {}};
}

template <ConstString s>
constexpr inline parse_result_t<string_result_t<get_ast_len<s>()>>
parse_string() {
    parse_result_t<string_result_t<get_ast_len<s>()>> result;
    result.is_valid = true;

    unsigned ast_position = 0;

    unsigned i = 0;
    while (i < s.size()) {
        if (s[i] == '{') {
            ++i;

            auto [is_valid, new_i, format_node] = parse_braces<s>(i);
            if (!is_valid) return {false, i, {}};
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


}} // namespace const_fmt::const_fmt_detail



#endif // LOGGER_PARSE_H
