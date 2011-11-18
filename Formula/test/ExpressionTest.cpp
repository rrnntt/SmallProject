#include "gtest/gtest.h"
#include "Formula/Variable.h"

#include "Formula/Expression.h"
#include "Formula/Bool.h"

#include <iostream>

using namespace Formula;

TEST(ExpressionTest, Namespace) 
{
  Namespace_ptr ns(new Namespace());
  ns->addVariable("Scalar","x");
  ns->addVariable("Scalar","y");
  ns->setVariable("x","23");
  ns->setVariable("y","32");
  Expression e(ns,"x+y");
  EXPECT_EQ(e.object()->eval().toString(),"55");

  ns->addVariable("Scalar","z");
  ns->setVariable("z","15");
  Expression e1(ns,"x+y-z");
  EXPECT_EQ(e1.object()->eval().toString(),"40");

  Expression e2(ns,"x+1");
  EXPECT_EQ(e2.object()->eval().toString(),"24");

  Expression e3(ns,"(z-5)^2");
  EXPECT_EQ(e3.object()->type(),"PowerFunction");
  EXPECT_EQ(e3.eval().toString(),"100");

  Expression e4(ns,"(z-5)^2/100");
  EXPECT_EQ(e4.object()->type(),"TimesFunction");
  EXPECT_EQ(e4.eval().toString(),"1");

  Expression e5(ns,"(z-5)^2/10+x*(3*y-2)");
  EXPECT_EQ(e5.eval().toString(),"2172");


}

  TEST(ExpressionTest,Boolean)
  {
    Namespace_ptr ns(new Namespace());
    ns->addVariable("Scalar","x","23");
    ns->addVariable("Scalar","x1","23");
    ns->addVariable("Scalar","y","32");
    ns->addVariable("Scalar","n");

    Expression e(ns,"x=y");
    EXPECT_EQ(e.eval().toString(),"False");

    Expression e1(ns,"x=x1");
    EXPECT_EQ(e1.eval().toString(),"True");

    Expression e2(ns,"x = y - 9");
    EXPECT_EQ(e2.eval().toString(),"True");

    Expression e2_1(ns,"x = - 9 + y");
    EXPECT_EQ(e2_1.eval().toString(),"True");

    Expression e3(ns,"x > x1");
    EXPECT_EQ(e3.eval().toString(),"False");

    Expression e4(ns,"x < x1");
    EXPECT_EQ(e4.eval().toString(),"False");

    Expression e5(ns,"x >= x1");
    EXPECT_EQ(e5.eval().toString(),"True");

    Expression e6(ns,"x <= x1");
    EXPECT_EQ(e6.eval().toString(),"True");

    Expression e7(ns,"x != y");
    EXPECT_EQ(e7.eval().toString(),"True");

    Bool True(true);

    Expression e8(ns,"x > x1 - 1");
    EXPECT_TRUE(True == e8.eval());

    Expression e9(ns,"x = n");
    EXPECT_EQ(e9.eval().toString(),"False");

  }