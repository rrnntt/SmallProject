#include "gtest/gtest.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceFactory.h"

#include <iostream>

TEST(VectorTest, Negative) {
  int i = 3;
  EXPECT_EQ(3, i);
  EXPECT_TRUE(i == 3);
}

