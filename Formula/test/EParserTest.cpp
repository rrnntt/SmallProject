#include "gtest/gtest.h"
#include "Formula/EParser.h"

#include <iostream>
#include <string>

using namespace Formula;

TEST(EParserTest, Simple) 
{
  EXPECT_EQ(1+1,2);
  std::cerr << "Hello!\n";
}

