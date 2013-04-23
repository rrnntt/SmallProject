#include "gtest/gtest.h"
#include "Formula/Parser.h"

#include <iostream>

TEST(ParserTest, Simple) 
{
  Formula::Parser p;
  p.parse("a+b");
  EXPECT_EQ(p.size(),2);
  EXPECT_EQ(p.str(),"(a+b)");
}

TEST(ParserTest, ExpFormReadAsNumber) 
{
  Formula::Parser p;
  p.parse("1.2e+2+1");
  EXPECT_EQ(p.size(),2);
  EXPECT_EQ(p[0].name(),"1.2e+2");
}
