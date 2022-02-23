#ifndef LOGGER_FORMAT_IMPL_H
#define LOGGER_FORMAT_IMPL_H


/*
 *
 * ****************************************************************
 * Disclaimer: Most of this code is shamelessly stolen from fmtlib
 * ****************************************************************
 *
 */

#include <stdint.h>

#include "stdlib.h"
#include "utility.h"


namespace const_fmt { namespace const_fmt_detail {


/*
 *
 * Utility functions
 *
 */


#define FMT_POWERS_OF_10(factor)                                               \
    factor * 10, (factor)*100, (factor)*1000, (factor)*10000, (factor)*100000, \
        (factor)*1000000, (factor)*10000000, (factor)*100000000,               \
        (factor)*1000000000

template <typename T>
constexpr int count_digits_decimal_fallback(T n) {
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

inline int do_count_digits_decimal(uint64_t n) {
    // Maps bsr(n) to ceil(log10(pow(2, bsr(n) + 1) - 1)).
    static constexpr uint8_t bsr2log10[] = {
        1,  1,  1,  2,  2,  2,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,
        6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  10, 10, 10,
        10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15,
        15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20};
    auto                            t = bsr2log10[__builtin_clzll(n | 1) ^ 63];
    static constexpr const uint64_t zero_or_powers_of_10[] = {
        0, 0, FMT_POWERS_OF_10(1U), FMT_POWERS_OF_10(1000000000ULL),
        10000000000000000000ULL};
    return t - (n < zero_or_powers_of_10[t]);
}

template <FormatType t_format_type>
constexpr inline auto count_digits_base(uint64_t n) -> int {
    if constexpr (t_format_type == FormatType::b) {
        int result = 0;

        while (n) {
            n = n >> 1;
            result += 1;
        }

        return result;
    } else {
        if constexpr (t_format_type == FormatType::x) {
            int result = 0;

            while (n) {
                n = n >> 4;
                result += 1;
            }

            return (result + count_digits_base<FormatType::b>(n));
        } else {
            if (!std::is_constant_evaluated()) {
                return do_count_digits_decimal(n);
            }
            return count_digits_decimal_fallback(n);
        }
    }
}


// Converts value in the range [0, base^2) to a string.
template <FormatType t_format_type>
constexpr inline const char* digits2_base(size_t value) {
    // GCC generates slightly better code when value is pointer-size.
    if constexpr (t_format_type == FormatType::b) {
        return &"00011011"[value * 2];
    } else {
        if constexpr (t_format_type == FormatType::x) {
            // clang-format off
            return &"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
                    "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
                    "404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
                    "606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F"
                    "808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9F"
                    "A0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
                    "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
                    "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF"[value * 2];
            // clang-format on
        } else {
            return &"0001020304050607080910111213141516171819"
                    "2021222324252627282930313233343536373839"
                    "4041424344454647484950515253545556575859"
                    "6061626364656667686970717273747576777879"
                    "8081828384858687888990919293949596979899"[value * 2];
        }
    }
}


constexpr inline void copy2(char* dst, const char* src) {
    if (!std::is_constant_evaluated()) {
        memcpy(dst, src, 2);
        return;
    }
    *dst++ = static_cast<char>(*src++);
    *dst   = static_cast<char>(*src);
}

template <FormatType t_format_type>
consteval inline unsigned get_base_divisor() {
    switch (t_format_type) {
    case FormatType::b:
        return 2;
    case FormatType::x:
        return 16;
    default:
        return 10;
    }
}

template <FormatType t_format_type, typename uint_t>
constexpr inline void format_base(char* out, uint_t value, int n_digits,
                                  int size) {
    constexpr unsigned divisor        = get_base_divisor<t_format_type>();
    constexpr unsigned square_divisor = const_pow(divisor, 2);

    if (n_digits > size) {
        for (int i = 0; i < size; ++i) {
            *(out++) = 'f';
        }
        return;
    }

    out += size;
    while (value >= square_divisor) {
        out -= 2;
        copy2(out, digits2_base<t_format_type>(
                       static_cast<size_t>(value % square_divisor)));
        value /= square_divisor;
    }

    if (value < divisor) {
        *--out = digits2_base<t_format_type>(value*divisor)[0];
        return;
    }

    out -= 2;
    copy2(out, digits2_base<t_format_type>(static_cast<size_t>(value)));
}


// returns {abs_value, was_negative}
template <std::signed_integral int_t>
constexpr std::pair<typename std::make_unsigned<int_t>::type, bool>
get_abs_value(int_t value) {
    using uint_t = typename std::make_unsigned<int_t>::type;

    uint_t abs_value = static_cast<uint_t>(value);

    const bool negative = value < 0;
    if (negative) abs_value = 0 - abs_value;

    return {abs_value, negative};
}

template <std::unsigned_integral int_t>
constexpr std::pair<int_t, bool> get_abs_value(int_t value) {
    return {value, false};
}


/*
 *
 * Integral types
 *
 */


template <fmt_data_t t_fmt_node, std::unsigned_integral uint_t>
constexpr inline void format_int(char* out, uint_t value) {
    format_base<t_fmt_node.type>(out, value,
                                 count_digits_base<t_fmt_node.type>(value),
                                 t_fmt_node.length);
}


template <fmt_data_t t_fmt_node, std::signed_integral int_t>
constexpr inline void format_int(char* out, int_t value) {
    const auto [abs_value, negative] = get_abs_value(value);

    const std::size_t n_digits = count_digits_base<t_fmt_node.type>(abs_value);

    format_base<t_fmt_node.type>(out + 1 * (negative), abs_value, n_digits,
                                 t_fmt_node.length - 1 * (negative));

    if constexpr (t_fmt_node.has_zero_padding) {
        if (negative) *(out) = '-';
    } else {
        if (n_digits < t_fmt_node.length)
            if (negative) *(out + t_fmt_node.length - n_digits - 1) = '-';
    }
}


/*
 *
 * Floating point types
 *
 */


template <fmt_data_t t_fmt_node, std::floating_point float_t>
constexpr inline void format_float(char* out, float_t value) {
    // clang-format off
    constexpr fmt_data_t fmt_node_integral = {
        t_fmt_node.has_zero_padding,                  // has_zero_padding
        t_fmt_node.length - t_fmt_node.precision - 1, // length
        t_fmt_node.precision,                         // ignored
        FormatType::d,                                // type
        t_fmt_node.position                           // ignored
    };
    constexpr fmt_data_t fmt_node_fractional = {
        true,                                         // has_zero_padding
        t_fmt_node.precision,                         // length
        t_fmt_node.precision,                         // ignored
        FormatType::d,                                // type
        t_fmt_node.position                           // ignored
    };
    // clang-format on

    *(out + t_fmt_node.length - t_fmt_node.precision - 1) = '.';

    const int integral = static_cast<int>(value);

    constexpr std::size_t factor = const_pow(10, t_fmt_node.precision);
    const int fractional = static_cast<int>((value - integral) * factor);

    const auto [fractional_abs, fractional_negative] =
        get_abs_value(fractional);

    format_int<fmt_node_integral, int>(out, integral);
    format_int<fmt_node_fractional, uint16_t>(
        out + t_fmt_node.length - t_fmt_node.precision, fractional_abs);
}


}} // namespace const_fmt::const_fmt_detail


#endif // LOGGER_FORMAT_IMPL_H
