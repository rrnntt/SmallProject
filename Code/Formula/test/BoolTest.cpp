#include "gtest/gtest.h"

#include "Formula/Bool.h"
#include "Formula/Expression.h"

#include <iostream>
#include <sstream>

using namespace Formula;

TEST(BoolEqualTest, Negative) {
  Bool z1,z2;
  EXPECT_TRUE(z1 == z2);

  Bool a(true),b(true);
  EXPECT_TRUE(a == b);
  EXPECT_EQ(a , true);
  EXPECT_NE(a , false);
  EXPECT_EQ(a.toString(),"True");
  EXPECT_EQ(z1.toString(),"False");
}

TEST(BoolFunctionsTest, Negative) {
  Namespace_ptr ns(new Namespace());
  ns->addVariable("Bool","T","True");
  ns->addVariable("Bool","T1","True");
  ns->addVariable("Bool","F","False");
  ns->addVariable("Bool","F1","False");

  Expression e1(ns,"T & T1");
  EXPECT_EQ(e1.eval().toString() , "True");

  //    Expression e2(ns,"T & T1 & True"); // Error
  //    EXPECT_EQ(e2.eval().toString() , "True");

  Expression e3(ns,"T & T1 & F");
  EXPECT_EQ(e3.eval().toString() , "False");

  Expression e4(ns,"T | F");
  EXPECT_EQ(e4.eval().toString() , "True");

  Expression e5(ns,"T | T1");
  EXPECT_EQ(e5.eval().toString() , "True");

  Expression e6(ns,"F | F1");
  EXPECT_EQ(e6.eval().toString() , "False");

  Expression e7(ns,"F | T");
  EXPECT_EQ(e7.eval().toString() , "True");

}

