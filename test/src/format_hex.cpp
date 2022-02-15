#include <const_fmt/format.h>
#include <gtest/gtest.h>


using namespace const_fmt;
using namespace const_fmt::const_fmt_detail;


TEST(FormatHex, positive_int) {
    constexpr std::array<char, 8> control1   = {'0', '0', '0', '0',
                                                '0', '0', '1', '0'};
    constexpr std::array<char, 8> formatted1 = const_format<"{:08x}">(0x10);

    constexpr std::array<char, 8> control2   = {' ', ' ', ' ', 'F',
                                                'F', 'A', '7', '6'};
    constexpr std::array<char, 8> formatted2 = const_format<"{:8x}">(0xffa76);

    constexpr std::array<char, 8> control3   = {'0', '0', '0', '0',
                                                'B', 'C', 'E', 'F'};
    constexpr std::array<char, 8> formatted3 = const_format<"{:08.4x}">(0xbcef);

    constexpr std::array<char, 4> control4   = {'A', 'D', '0', '1'};
    constexpr std::array<char, 4> formatted4 = const_format<"{:4x}">(0xad01);

    constexpr std::array<char, 4> control5   = {'f', 'f', 'f', 'f'};
    constexpr std::array<char, 4> formatted5 = const_format<"{:4x}">(0x12345);

    EXPECT_EQ(control1, formatted1);
    EXPECT_EQ(control2, formatted2);
    EXPECT_EQ(control3, formatted3);
    EXPECT_EQ(control4, formatted4);
    EXPECT_EQ(control5, formatted5);
}

TEST(FormatHex, negative_int) {
    constexpr std::array<char, 8> control1   = {'-', '0', '0', '0',
                                                '0', '0', '1', '0'};
    constexpr std::array<char, 8> formatted1 = const_format<"{:08x}">(-0x10);

    constexpr std::array<char, 8> control2   = {' ', ' ', '-', 'F',
                                                'F', 'A', '7', '6'};
    constexpr std::array<char, 8> formatted2 = const_format<"{:8x}">(-0xffa76);

    constexpr std::array<char, 8> control3   = {'-', '0', '0', '0',
                                                'B', 'C', 'E', 'F'};
    constexpr std::array<char, 8> formatted3 = const_format<"{:08.4x}">(-0xbcef);

    constexpr std::array<char, 4> control4   = {'-', 'A', 'D', '1'};
    constexpr std::array<char, 4> formatted4 = const_format<"{:4x}">(-0xad1);

    constexpr std::array<char, 4> control5   = {'-', 'f', 'f', 'f'};
    constexpr std::array<char, 4> formatted5 = const_format<"{:04x}">(-0x1234);

    EXPECT_EQ(control1, formatted1);
    EXPECT_EQ(control2, formatted2);
    EXPECT_EQ(control3, formatted3);
    EXPECT_EQ(control4, formatted4);
    EXPECT_EQ(control5, formatted5);
}
