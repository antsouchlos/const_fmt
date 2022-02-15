#include <const_fmt/format_impl.h>
#include <limits>
#include <gtest/gtest.h>


using namespace const_fmt;
using namespace const_fmt::const_fmt_detail;


TEST(FormatUtility, count_digits_base_decimal) {
    constexpr unsigned length1 = count_digits_base<FormatType::d>(123);
    constexpr unsigned length2 = count_digits_base<FormatType::d>(std::numeric_limits<uint64_t>::max());
    constexpr unsigned length3 = count_digits_base<FormatType::d>(10000011);
    constexpr unsigned length4 = count_digits_base<FormatType::d>(1);

    EXPECT_EQ(length1, 3);
    EXPECT_EQ(length2, 20);
    EXPECT_EQ(length3, 8);
    EXPECT_EQ(length4, 1);
}

TEST(FormatUtility, count_digits_base_binary) {
    constexpr unsigned length1 = count_digits_base<FormatType::b>(0b1001);
    constexpr unsigned length2 = count_digits_base<FormatType::b>(std::numeric_limits<uint64_t>::max());
    constexpr unsigned length3 = count_digits_base<FormatType::b>(0b10000001);
    constexpr unsigned length4 = count_digits_base<FormatType::b>(0b01);

    EXPECT_EQ(length1, 4);
    EXPECT_EQ(length2, 64);
    EXPECT_EQ(length3, 8);
    EXPECT_EQ(length4, 1);
}

TEST(FormatUtility, count_digits_base_hex) {
    constexpr unsigned length1 = count_digits_base<FormatType::x>(0x123);
    constexpr unsigned length2 = count_digits_base<FormatType::x>(std::numeric_limits<uint64_t>::max());
    constexpr unsigned length3 = count_digits_base<FormatType::x>(0x1000000f);
    constexpr unsigned length4 = count_digits_base<FormatType::x>(0x01);

    EXPECT_EQ(length1, 3);
    EXPECT_EQ(length2, 16);
    EXPECT_EQ(length3, 8);
    EXPECT_EQ(length4, 1);
}
