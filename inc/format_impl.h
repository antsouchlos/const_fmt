#ifndef LOGGER_FORMAT_IMPL_H
#define LOGGER_FORMAT_IMPL_H


/*
 *
 * ****************************************************************
 * Disclaimer: Most of this code is shamelessly stolen from fmtlib
 * ****************************************************************
 *
 */


#include <cstdint>

#include "utility.h"


namespace detail {


/*
 *
 * Utility functions
 *
 */


#define FMT_POWERS_OF_10(factor)                                             \
  factor * 10, (factor)*100, (factor)*1000, (factor)*10000, (factor)*100000, \
      (factor)*1000000, (factor)*10000000, (factor)*100000000,               \
      (factor)*1000000000


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

template <typename uint_t>
constexpr inline void format_decimal(char* out, uint_t value, int size) {
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

template <typename T>
constexpr int count_digits_fallback(T n) {
    int count = 1;
    for (;;) {
        if (n < 10) return count;
        if (n < 100) return count + 1;
        if (n < 1000) return count + 2;
        if (n < 10000) return count + 3;
        n /= 10000u;
        count += 4;
    }
}

inline int do_count_digits(uint64_t n) {
  // Maps bsr(n) to ceil(log10(pow(2, bsr(n) + 1) - 1)).
  static constexpr uint8_t bsr2log10[] = {
      1,  1,  1,  2,  2,  2,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,
      6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  10, 10, 10,
      10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15,
      15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20};
  auto t = bsr2log10[__builtin_clzll(n | 1) ^ 63];
  static constexpr const uint64_t zero_or_powers_of_10[] = {
      0, 0, FMT_POWERS_OF_10(1U), FMT_POWERS_OF_10(1000000000ULL),
      10000000000000000000ULL};
  return t - (n < zero_or_powers_of_10[t]);
}

constexpr inline auto count_digits(uint64_t n) -> int {
    if (!std::is_constant_evaluated()) {
        return do_count_digits(n);
    }
    return count_digits_fallback(n);
}


/*
 *
 * Integral types
 *
 */


template <std::unsigned_integral uint_t>
constexpr inline void format_int(char* out, uint_t value, fmt_data_t fmt_node) {
    format_decimal(out, value, fmt_node.length);
}

template <std::signed_integral uint_t>
constexpr inline void format_int(char* out, uint_t value, fmt_data_t fmt_node) {
    if (value < 0) value = -value;
    format_decimal(out, value, fmt_node.length);
}


/*
 *
 * Floating point types
 *
 */


template <std::floating_point float_t>
constexpr inline void format_float(char* out, float_t value,
                                   fmt_data_t fmt_node) {
    // TODO
}


} // namespace detail


#endif // LOGGER_FORMAT_IMPL_H
