#include "gtest/gtest.h"
#include "Numeric/UserFunction1D.h"

#include <iostream>

using namespace Numeric;

TEST(UserFunction1DTest, Test)
{
  UserFunction1D fun;
  fun.setAttributeValue("Formula","a+ b * x");
  auto names = fun.getParameterNames();
  EXPECT_EQ(names.size(),2);
  EXPECT_EQ(names[0],"a");
  EXPECT_EQ(names[1],"b");
  fun.setParameter("a",1.0);
  fun.setParameter("b",2.0);
  EXPECT_EQ(fun.getParameter("a"),1.0);
  EXPECT_EQ(fun.getParameter("b"),2.0);

  FunctionDomain1DVector x(0,10,11);
  FunctionValues y(x);
  fun.function(x,y);
  for(size_t i = 0; i < y.size(); ++i)
  {
    EXPECT_EQ(y.getCalculated(i), 1.0 + 2.0 * x[i]);
    EXPECT_NE(y.getCalculated(i),0.0);
  }
}

