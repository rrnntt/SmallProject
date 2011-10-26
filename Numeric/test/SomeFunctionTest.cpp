#include "gtest/gtest.h"
#include "nu/SomeFunction.h"

TEST(FactorialTest, Negative) {
  int i = 3;
  EXPECT_EQ(3, i);
  EXPECT_TRUE(i == 3);
  EXPECT_EQ(i, SomeFunction());
}

