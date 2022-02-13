#include <gtest/gtest.h>
#include <const_fmt/parse.h>


using namespace const_fmt::const_fmt_detail;


TEST(Parse, parse_number) {
    constexpr auto pr1 = parse_number<"0">(0);
    constexpr auto pr2 = parse_number<"9">(0);
    constexpr auto pr3 = parse_number<"abcd16777216">(4);
    constexpr auto pr4 = parse_number<"009">(0);
    constexpr auto pr5 = parse_number<"10101010109">(6);
    constexpr auto pr6 = parse_number<"">(0);

    EXPECT_EQ(pr1.value, 0);
    EXPECT_EQ(pr2.value, 9);
    EXPECT_EQ(pr3.value, 16777216);
    EXPECT_EQ(pr4.value, 9);
    EXPECT_EQ(pr5.value, 10109);
    EXPECT_EQ(pr6.is_valid, false);
}

TEST(Parse, ast_node) {
    constexpr ast_node_t ast_node1{'a'};
    constexpr ast_node_t ast_node2{'a'};
    constexpr ast_node_t ast_node3{'b'};

    EXPECT_EQ(ast_node1 == ast_node2, true);
    EXPECT_EQ(ast_node1 == ast_node3, false);
}

TEST(Parse, ast_chars_only) {
    constexpr std::array<ast_node_t, 8> control = {'a', 'b', 'c', 'd',
                                                   'e', 'f', 'g', 'h'};

    EXPECT_EQ(parse_string<"abcdefgh">().value, control);
}

TEST(Parse, syntax) {
    constexpr auto pr1 = parse_string<"af {:012.4f}">();
    constexpr auto pr2 = parse_string<"af {:.4f}">();
    constexpr auto pr3 = parse_string<"asdf}{">();
    constexpr auto pr4 = parse_string<"{:.}">();
    constexpr auto pr5 = parse_string<"asdf{">();
    constexpr auto pr6 = parse_string<"asd{d}:.8">();

    EXPECT_EQ(pr1.is_valid, true);
    EXPECT_EQ(pr2.is_valid, true);
    EXPECT_EQ(pr3.is_valid, false);
    EXPECT_EQ(pr4.is_valid, false);
    EXPECT_EQ(pr5.is_valid, false);
    EXPECT_EQ(pr6.is_valid, false);
}

TEST(Parse, ast_nodes_only) {
    fmt_node_t fmt_node0 = {true, 6, 3, FormatType::s};

    fmt_node_t fmt_node1;
    fmt_node1.length = 8;
    fmt_node1.type   = FormatType::s;

    fmt_node_t fmt_node2;
    fmt_node2.precision = 4;

    fmt_node_t fmt_node3 = {true, 456765, 1234, FormatType::f};
    fmt_node_t fmt_node4 = {};
    fmt_node_t fmt_node5 = {};
    fmt_node_t fmt_node6 = {};
    fmt_node_t fmt_node7 = {};

    std::array<ast_node_t, 8> control = {fmt_node0, fmt_node1, fmt_node2,
                                         fmt_node3, fmt_node4, fmt_node5,
                                         fmt_node6, fmt_node7};

    EXPECT_EQ(parse_string<"{:06.3}{:8s}{:.4}{:0456765.1234f}{}{}{}{}">().value,
              control);
}

TEST(Parse, ast_chars_and_nodes) {
    fmt_node_t fmt_node0 = {true, 6, 3, FormatType::s};

    fmt_node_t fmt_node1;
    fmt_node1.length = 8;
    fmt_node1.type   = FormatType::s;

    fmt_node_t fmt_node2;
    fmt_node2.precision = 4;


    std::array<ast_node_t, 11> control = {fmt_node0, 'a', 'b',       ' ',
                                         ' ',       'd', fmt_node1, fmt_node2,
                                         ' ',       ' ', '-'};

    EXPECT_EQ(parse_string<"{:06.3}ab  d{:8s}{:.4}  -">().value, control);
}
