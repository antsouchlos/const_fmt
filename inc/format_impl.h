#ifndef LOGGER_FORMAT_IMPL_H
#define LOGGER_FORMAT_IMPL_H


/*
 *
 * ****************************************************************
 * Disclaimer: Most of this code is shamelessly stolen from fmtlib
 * ****************************************************************
 *
 */


#include "utility.h"


namespace detail {


/*
 *
 * Utility functions
 *
 */


// Converts value in the range [0, 100) to a string.
constexpr inline const char* digits2(size_t value) {
    // GCC generates slightly better code when value is pointer-size.
    return &"0001020304050607080910111213141516171819"
            "2021222324252627282930313233343536373839"
            "4041424344454647484950515253545556575859"
            "6061626364656667686970717273747576777879"
            "8081828384858687888990919293949596979899"[value * 2];
}


constexpr inline void copy2(char* dst, const char* src) {
    if (!std::is_constant_evaluated()) {
        std::memcpy(dst, src, 2);
        return;
    }

    *dst++ = static_cast<char>(*src++);
    *dst   = static_cast<char>(*src);
}

template <typename UInt>
constexpr inline void format_decimal(char* out, UInt value, int size) {
    out += size;
    while (value >= 100) {
        out -= 2;
        copy2(out, digits2(static_cast<size_t>(value % 100)));
        value /= 100;
    }

    if (value < 10) *--out = static_cast<char>('0' + value);

    out -= 2;
    copy2(out, digits2(static_cast<size_t>(value)));
}


/*
 *
 * Integral types
 *
 */


template <std::unsigned_integral uint_t>
constexpr inline void format_integral(char* out, uint_t value,
                                      fmt_data_t fmt_node) {
    format_decimal(out, value, fmt_node.length);
}

template <std::signed_integral uint_t>
constexpr inline void format_integral(char* out, uint_t value,
                                      fmt_data_t fmt_node) {
    if (value < 0) value = -value;
    format_decimal(out, value, fmt_node.length);
}


/*
 *
 * Floating point types
 *
 */


template <std::floating_point float_t>
constexpr inline void format_floating_point(char* out, float_t value,
                                      fmt_data_t fmt_node) {
    // TODO
}


} // namespace detail


#endif // LOGGER_FORMAT_IMPL_H
