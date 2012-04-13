#include "gtest/gtest.h"
#include "Kernel/ParsedExpressionString.h"

using namespace Kernel;

TEST(Kernel_ParsedExpressionString_Test, FunctionTest)
{
  ParsedExpressionString ps("Function(x,y)");
  EXPECT_EQ(static_cast<std::string>(ps), "Function(x,y)");
  EXPECT_EQ(ps.parser(4).name(), "Function");
  EXPECT_EQ(ps.parser(9).name(), "x");
  EXPECT_EQ(ps.parser(10).name(), "Function");
  EXPECT_EQ(ps.parser(11).name(), "y");
  EXPECT_EQ(ps.parser(12).name(), "Function");
}

TEST(Kernel_ParsedExpressionString_Test, SumTest)
{
  ParsedExpressionString ps("x+y+sin(z)");
  EXPECT_EQ(static_cast<std::string>(ps), "x+y+sin(z)");
  EXPECT_EQ(ps.parser(0).name(), "x");
  EXPECT_EQ(ps.parser(1).name(), "+");
  EXPECT_EQ(ps.parser(2).name(), "y");
  EXPECT_EQ(ps.parser(3).name(), "+");
  EXPECT_EQ(ps.parser(5).name(), "sin");
  EXPECT_EQ(ps.parser(8).name(), "z");

  EXPECT_EQ(ps.parser().parentOf(ps.parser(8)).name(), "sin");
  EXPECT_EQ(ps.parser().parentOf(ps.parser(5)).name(), "+");
  EXPECT_EQ(ps.parser().parentOf(ps.parser(3)).name(), "+");
}

TEST(Kernel_ParsedExpressionString_Test, Function1Test)
{
  ParsedExpressionString ps("Function(fun(x=2),g(y=1))");
  EXPECT_EQ(static_cast<std::string>(ps), "Function(x,y)");
  EXPECT_EQ(ps.parser(15).name(), "=");
}

