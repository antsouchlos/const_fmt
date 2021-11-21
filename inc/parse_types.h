#ifndef LOGGER_PARSE_TYPES_H
#define LOGGER_PARSE_TYPES_H


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
};


class ast_node_t {
public:
    constexpr ast_node_t() {
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


#endif // LOGGER_PARSE_TYPES_H