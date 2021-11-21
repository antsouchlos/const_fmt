#include <utility.h>
#include <gtest/gtest.h>

TEST(Utility, const_pow) {
    EXPECT_EQ(detail::const_pow(10, 0), 1);
    EXPECT_EQ(detail::const_pow(0, 1), 0);
    EXPECT_EQ(detail::const_pow(2, 8), 0b1'0000'0000);
}

