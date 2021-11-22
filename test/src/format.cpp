#include <format.h>
#include <gtest/gtest.h>


using namespace detail;


TEST(Format, positive_int) {
    constexpr std::array<char, 8> control1   = {'0', '0', '0', '2',
                                                '2', '2', '2', '2'};
    constexpr std::array<char, 8> formatted1 = format<"{:08}">(22222);

    constexpr std::array<char, 8> control2   = {' ', ' ', ' ', '2',
                                                '2', '2', '2', '2'};
    constexpr std::array<char, 8> formatted2 = format<"{:8}">(22222);

    constexpr std::array<char, 8> control3   = {'0', '0', '0', '1',
                                                '2', '3', '4', '5'};
    constexpr std::array<char, 8> formatted3 = format<"{:08.4}">(12345);

    constexpr std::array<char, 4> control4   = {'6', '7', '8', '9'};
    constexpr std::array<char, 4> formatted4 = format<"{:4}">(6789);

    EXPECT_EQ(control1, formatted1);
    EXPECT_EQ(control2, formatted2);
    EXPECT_EQ(control3, formatted3);
    EXPECT_EQ(control4, formatted4);
}

//TEST(Format, negative_int) {
//    // TODO
//    EXPECT_EQ(true, false);
//}
//
//TEST(Format, positive_float) {
//    // TODO
//    EXPECT_EQ(true, false);
//}
//
//TEST(Format, negative_float) {
//    // TODO
//    EXPECT_EQ(true, false);
//}
//
//TEST(Format, string) {
//    // TODO
//    EXPECT_EQ(true, false);
//}