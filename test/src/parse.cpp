#include <gtest/gtest.h>
#include <parse.h>


using namespace detail;


TEST(Parse, ast_node) {
    ast_node_t ast_node1{'a'};
    ast_node_t ast_node2{'a'};
    ast_node_t ast_node3{'b'};

    EXPECT_EQ(ast_node1 == ast_node2, true);
    EXPECT_EQ(ast_node1 == ast_node3, false);
}

TEST(Parse, ast_chars_only) {
    constexpr std::array<ast_node_t, 8> control = {'a', 'b', 'c', 'd',
                                                   'e', 'f', 'g', 'h'};

    EXPECT_EQ(parse_string<"abcdefgh">().value, control);
}

TEST(Parse, ast_nodes_only) {
    fmt_node_t fmt_node0 = {true, 6, 3, FormatType::s};

    fmt_node_t fmt_node1;
    fmt_node1.length = 8;
    fmt_node1.type = FormatType::s;

    fmt_node_t fmt_node2;
    fmt_node2.precision = 4;

    fmt_node_t fmt_node3 = {};
    fmt_node_t fmt_node4 = {};
    fmt_node_t fmt_node5 = {};
    fmt_node_t fmt_node6 = {};
    fmt_node_t fmt_node7 = {};

    std::array<ast_node_t, 8> control = {
        fmt_node0, fmt_node1, fmt_node2, fmt_node3,
        fmt_node4, fmt_node5, fmt_node6, fmt_node7};

    EXPECT_EQ(parse_string<"{:06.3}{:8s}{:.4}{}{}{}{}{}">().value, control);
}
