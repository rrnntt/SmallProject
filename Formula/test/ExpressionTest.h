#ifndef EXPRESSIONTEST_H
#define EXPRESSIONTEST_H

#include "../Expression.h"
#include "../Bool.h"
#include "TestSuite.h"

#include <iostream>

using namespace Formula;

class ExpressionTest
{
public:
  static void test()
  {
    ExpressionTest p;
    p.testNamespace();
    p.testParser();
    p.testBoolean();
  }

  void testNamespace()
  {
    Namespace_ptr ns(new Namespace());
    ns->addVariable("Scalar","x");
    ns->addVariable("Scalar","y");
    ns->setVariable("x","23");
    ns->setVariable("y","32");
    Expression e(ns,"x+y");
    TS_ASSERT_EQUALS(e.object()->eval().toString(),"55");

    ns->addVariable("Scalar","z");
    ns->setVariable("z","15");
    Expression e1(ns,"x+y-z");
    TS_ASSERT_EQUALS(e1.object()->eval().toString(),"40");

    Expression e2(ns,"x+1");
    TS_ASSERT_EQUALS(e2.object()->eval().toString(),"24");

    Expression e3(ns,"(z-5)^2");
    TS_ASSERT_EQUALS(e3.object()->type(),"PowerFunction");
    TS_ASSERT_EQUALS(e3.eval().toString(),"100");

    Expression e4(ns,"(z-5)^2/100");
    TS_ASSERT_EQUALS(e4.object()->type(),"TimesFunction");
    TS_ASSERT_EQUALS(e4.eval().toString(),"1");

    Expression e5(ns,"(z-5)^2/10+x*(3*y-2)");
    TS_ASSERT_EQUALS(e5.eval().toString(),"2172");


  }

  void testParser()
  {
    Parser p;
    p.parse("Type :name");
  }

  void testBoolean()
  {
    Namespace_ptr ns(new Namespace());
    ns->addVariable("Scalar","x","23");
    ns->addVariable("Scalar","x1","23");
    ns->addVariable("Scalar","y","32");
    ns->addVariable("Scalar","n");

    Expression e(ns,"x=y");
    TS_ASSERT_EQUALS(e.eval().toString(),"False");

    Expression e1(ns,"x=x1");
    TS_ASSERT_EQUALS(e1.eval().toString(),"True");

    Expression e2(ns,"x = y - 9");
    TS_ASSERT_EQUALS(e2.eval().toString(),"True");

    Expression e3(ns,"x > x1");
    TS_ASSERT_EQUALS(e3.eval().toString(),"False");

    Expression e4(ns,"x < x1");
    TS_ASSERT_EQUALS(e4.eval().toString(),"False");

    Expression e5(ns,"x >= x1");
    TS_ASSERT_EQUALS(e5.eval().toString(),"True");

    Expression e6(ns,"x <= x1");
    TS_ASSERT_EQUALS(e6.eval().toString(),"True");

    Expression e7(ns,"x != y");
    TS_ASSERT_EQUALS(e7.eval().toString(),"True");

    Bool True(true);

    Expression e8(ns,"x > x1 - 1");
    TS_ASSERT_EQUALS(True,e8.eval());

    Expression e9(ns,"x = n");
    TS_ASSERT_EQUALS(e9.eval().toString(),"False");

  }
};

#endif // EXPRESSIONTEST_H
