#include <const_fmt/format.h>
#include <gtest/gtest.h>


using namespace const_fmt;
using namespace const_fmt::const_fmt_detail;


TEST(FormatBinary, positive_int) {
    constexpr std::array<char, 8> control1   = {'0', '0', '0', '0',
                                              '0', '0', '1', '0'};
    constexpr std::array<char, 8> formatted1 = const_format<"{:08b}">(0b10);

    constexpr std::array<char, 8> control2   = {' ', ' ', '1', '0',
                                              '1', '0', '1', '0'};
    constexpr std::array<char, 8> formatted2 = const_format<"{:8b}">(0b101010);

    constexpr std::array<char, 8> control3   = {'0', '0', '0', '1',
                                              '1', '0', '0', '1'};
    constexpr std::array<char, 8> formatted3 = const_format<"{:08.4b}">(0b11001);

    constexpr std::array<char, 4> control4   = {'1', '0', '1', '1'};
    constexpr std::array<char, 4> formatted4 = const_format<"{:4b}">(0b1011);

    constexpr std::array<char, 4> control5   = {'f', 'f', 'f', 'f'};
    constexpr std::array<char, 4> formatted5 = const_format<"{:4b}">(0b10011);

    EXPECT_EQ(control1, formatted1);
    EXPECT_EQ(control2, formatted2);
    EXPECT_EQ(control3, formatted3);
    EXPECT_EQ(control4, formatted4);
    EXPECT_EQ(control5, formatted5);
}

TEST(FormatBinary, negative_int) {
    constexpr std::array<char, 8> control1   = {'-', '0', '0', '0',
                                              '0', '0', '1', '0'};
    constexpr std::array<char, 8> formatted1 = const_format<"{:08b}">(-0b10);

    constexpr std::array<char, 8> control2   = {' ', '-', '1', '0',
                                                '1', '0', '1', '0'};
    constexpr std::array<char, 8> formatted2 = const_format<"{:8b}">(-0b101010);

    constexpr std::array<char, 8> control3   = {'-', '0', '0', '1',
                                              '0', '0', '1', '1'};
    constexpr std::array<char, 8> formatted3 = const_format<"{:08.4b}">(-0b10011);

    constexpr std::array<char, 5> control4   = {'-', '1', '1', '0', '1'};
    constexpr std::array<char, 5> formatted4 = const_format<"{:5b}">(-0b1101);

    constexpr std::array<char, 5> control5   = {'-', 'f', 'f', 'f', 'f'};
    constexpr std::array<char, 5> formatted5 = const_format<"{:05b}">(-0b10101);

    EXPECT_EQ(control1, formatted1);
    EXPECT_EQ(control2, formatted2);
    EXPECT_EQ(control3, formatted3);
    EXPECT_EQ(control4, formatted4);
    EXPECT_EQ(control5, formatted5);
}
