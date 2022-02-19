#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H



#include "std_lib.h"


namespace const_fmt { namespace const_fmt_detail {


/*
 *
 * Types used everywhere
 *
 */


template <std::size_t N>
class ConstString {
public:
    constexpr ConstString(const char (&content)[N]) noexcept {
        std::copy(&content[0], (&content[N - 1] + 1), m_content.begin());
    }

    constexpr char operator[](std::size_t index) const noexcept {
        return m_content[index];
    }

    constexpr std::size_t size() const noexcept {
        return N - 1;
    }

    std::array<char, N> m_content;
};


template <std::size_t N>
ConstString(const char (&)[N]) -> ConstString<N>;


/*
 *
 * Types used mainly in parsing
 *
 */


enum class FormatType { s, c, b, B, d, o, x, X, a, A, e, E, f, F, g, G, p };


template <typename value_t>
struct parse_result_t {
    bool     is_valid  = false;
    unsigned new_index = 0;
    value_t  value;
};


struct fmt_node_t {
    bool       has_zero_padding = false;
    int        length           = 6;
    int        precision        = 2;
    FormatType type             = FormatType::s;

    constexpr bool operator==(const fmt_node_t& rhs) const {
        return (rhs.has_zero_padding == has_zero_padding) &&
               (rhs.length == length) && (rhs.precision == precision) &&
               (rhs.type == type);
    }
};


class ast_node_t {
public:
    constexpr ast_node_t() {
    }

    constexpr ast_node_t(char c) : m_is_char(true), m_c(c) {
    }

    constexpr ast_node_t(fmt_node_t node) : m_is_char(false), m_node(node) {
    }

    constexpr bool operator==(const ast_node_t& rhs) const {
        return ((rhs.m_is_char == m_is_char) && (rhs.m_c == m_c)) ||
               ((rhs.m_is_char != m_is_char) && (rhs.m_node == m_node));
    }

    constexpr void set_char(char c) {
        m_c       = c;
        m_is_char = true;
    }

    constexpr void set_node(fmt_node_t node) {
        m_node    = node;
        m_is_char = false;
    }

    constexpr char get_char() const {
        return m_c;
    }

    constexpr fmt_node_t get_node() const {
        return m_node;
    }

    constexpr bool is_char() const {
        return m_is_char;
    }

    bool       m_is_char = false;
    char       m_c       = 'c';
    fmt_node_t m_node;
};


template <std::size_t N>
using string_result_t = std::array<ast_node_t, N>;


/*
 *
 * Types used mainly in formatting
 *
 */


struct fmt_data_t {
    bool        has_zero_padding = 0;
    std::size_t length           = 0;
    std::size_t precision        = 0;
    FormatType  type;

    std::size_t position = 0;
};


}} // namespace const_fmt::const_fmt_detail


#endif // LOGGER_TYPES_H
