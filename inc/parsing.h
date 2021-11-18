#ifndef LOGGER_PARSING_H
#define LOGGER_PARSING_H


#include <utility.h>


namespace detail {


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

template <std::size_t N>
constexpr std::pair<unsigned, int> parse_number(ConstString<N> s, unsigned i) {
    while ((i < s.size()) && is_digit(s, i)) {
        ++i;
    }

    return {i, 0};
}

template <std::size_t N>
constexpr std::pair<unsigned, int> parse_type(ConstString<N> s, unsigned i) {
    if (s[i] == 's') { // string
        ++i;
        return {i, 0};
    } else if (s[i] == 'c') { // char
        ++i;
        return {i, 0};
    } else if (s[i] == 'b') { // int
        ++i;
        return {i, 0};
    } else if (s[i] == 'B') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'c') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'd') {
        ++i;
        return {i, 0};
    } else if (s[i] == '0') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'x') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'X') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'a') { // float
        ++i;
        return {i, 0};
    } else if (s[i] == 'A') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'e') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'E') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'f') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'F') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'g') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'G') {
        ++i;
        return {i, 0};
    } else if (s[i] == 'p') { // pointer
        ++i;
        return {i, 0};
    }

    return {i, -1};
}

template <std::size_t N>
constexpr std::pair<unsigned, int> parse_fmt_string(ConstString<N> s,
                                                    unsigned       i) {
    int result_extra_len = 0;

    if (s[i] == '0')
        ++i;

    if (is_digit(s, i)) {
        auto [new_i, extra_len] = parse_number(s, i);
        if (extra_len < 0)
            return {i, -1};
        i = new_i;
        result_extra_len += extra_len;
    }

    if (s[i] == '.') {
        ++i;
        auto [new_i, extra_len] = parse_number(s, i);
        if (extra_len < 0)
            return {i, -1};
        i = new_i;
        result_extra_len += extra_len;
    }

    if (s[i] != '}') {
        auto [new_i, extra_len] = parse_type(s, i);
        if (extra_len < 0)
            return {i, -1};
        i = new_i;
        result_extra_len += extra_len;
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
        if (extra_len < 0)
            return {i, -1};
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
            if (extra_len < 0)
                return -1;
            i = new_i;
            result_extra_len += extra_len;

        } else if (s[i] == '}') {
            return -1;
        }
    }

    return (result_extra_len + s.size());
}


} // namespace detail


#endif // LOGGER_PARSING_H
