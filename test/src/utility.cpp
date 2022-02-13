#include <const_fmt/utility.h>
#include <gtest/gtest.h>

using namespace const_fmt;

TEST(Utility, const_pow) {
    EXPECT_EQ(const_fmt_detail::const_pow(10, 0), 1);
    EXPECT_EQ(const_fmt_detail::const_pow(0, 1), 0);
    EXPECT_EQ(const_fmt_detail::const_pow(2, 8), 0b1'0000'0000);
}

