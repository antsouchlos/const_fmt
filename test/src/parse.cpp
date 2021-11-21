#include <gtest/gtest.h>
#include <parse.h>

TEST(Parse, ast_node) {
    ast_node_t ast_node1{'a'};
    ast_node_t ast_node2{'a'};
    ast_node_t ast_node3{'b'};

    EXPECT_EQ(ast_node1 == ast_node2, true);
    EXPECT_EQ(ast_node1 == ast_node3, false);
}

TEST(Parse, chars_only) {
    constexpr std::array<ast_node_t, 8> control = {'a', 'b', 'c', 'd',
                                                   'e', 'f', 'g', 'h'};

    EXPECT_EQ(detail::parse_string<"abcdefgh">().value, control);
}
