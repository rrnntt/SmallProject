#include "gtest/gtest.h"
#include "nu/Vector.h"

#include <iostream>

TEST(VectorTest, Negative) {
  int i = 3;
  EXPECT_EQ(3, i);
  EXPECT_TRUE(i == 3);
  nu::Vector v(3);
}
