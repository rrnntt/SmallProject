#include "gtest/gtest.h"
#include "Numeric/BackToBackExponential.h"
#include "Numeric/FunctionFactory.h"

#include <iostream>

using namespace Numeric;

TEST(BackToBackExponentialTest, Test)
{
  BackToBackExponential fun;
  fun.setParameter("a",1.0);
  fun.setParameter("b",0.5);
  fun.setParameter("s",1.0);
  EXPECT_EQ(fun.getParameter("a"),1.0);
  EXPECT_EQ(fun.getParameter("b"),0.5);

  FunctionDomain1DVector x(-5.0,5.0,100);
  FunctionValues y(x);
  fun.function(x,y);
  for(size_t i = 0; i < y.size(); ++i)
  {
    EXPECT_EQ(y.getCalculated(i), 1.0 + 2.0 * x[i]);
    EXPECT_NE(y.getCalculated(i),0.0);
  }
}

TEST(BackToBackExponentialTest, FactoryTest)
{
  EXPECT_NO_THROW(FunctionFactory::instance().createFitFunction("UserFunction1D(Formula=a+ b * x)"));
  EXPECT_NO_THROW(FunctionFactory::instance().createFitFunction("UserFunction1D(Formula=a+ b * x,a=1)"));
}
