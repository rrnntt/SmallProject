#ifndef SCALARTEST_H
#define SCALARTEST_H

#include "../Scalar.h"
#include "../FunctionFactory.h"
#include "../VariableFactory.h"
#include "TestSuite.h"

#include <iostream>
#include <sstream>

using namespace Formula;

class ScalarTest
{
public:
  static void test()
  {
    ScalarTest p;
    p.testEqual();
    p.testNegate();
    p.testScalarFunctions();
    p.testScalarDeclaration();
  }

  void testEqual()
  {
    Scalar z1,z2;
    TS_ASSERT(z1 == z2);

    Scalar a(10.1),b(10.1);
    TS_ASSERT(a == b);
    TS_ASSERT_EQUALS(a , 10.1);
    TS_ASSERT_DIFFERS(a , 101);
    TS_ASSERT_EQUALS(a.toString(),"10.1");

  }
  void testNegate()
  {
    Scalar a(10.1);
    Scalar c(a);
    TS_ASSERT(a == c);
    c.negate();
    TS_ASSERT_EQUALS(c,-10.1);

    Scalar b;
    TS_ASSERT(b == 0);
    b.negate(a);
    TS_ASSERT_EQUALS(b,-10.1);
  }

  void testScalarFunctions()
  {
    std::vector<Argument> args;
    for(int i=0;i<3;++i)
    {
      Scalar* v = new Scalar;
      v->setValue((i+1)*2);
      args.push_back(Argument(v));
    }
    Function* fun = FunctionFactory::instance().create("+",args);
    std::vector<std::string> keys = FunctionFactory::instance().keys();
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"12");
    TS_ASSERT_EQUALS((*fun)().type(),"Scalar");
    delete fun;

    args.clear();
    args.push_back(Argument(new Scalar(2.0)));
    fun = FunctionFactory::instance().create("-",args);
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"-2");
    TS_ASSERT_EQUALS((*fun).type(),"UnaryMinusFunction");
    delete fun;

    args.clear();
    args.push_back(Argument(new Scalar(2.0)));
    args.push_back(Argument(new Scalar(5.0)));
    fun = FunctionFactory::instance().create("^",args);
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"32");
    TS_ASSERT_EQUALS((*fun).type(),"PowerFunction");
    delete fun;

    args.clear();
    args.push_back(Argument(new Scalar(1.0)));  // +
    args.push_back(Argument(new Scalar(1.0)));  // +
    args.push_back(Argument(new Scalar(3.0),1));// -
    fun = FunctionFactory::instance().create("+",args);
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"-1");
    TS_ASSERT_EQUALS((*fun).type(),"PlusFunction");
    delete fun;

    args.clear();
    args.push_back(Argument(new Scalar(2.0)));  // *
    args.push_back(Argument(new Scalar(2.0)));  // *
    args.push_back(Argument(new Scalar(5.0),1));// /
    fun = FunctionFactory::instance().create("*",args);
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"0.8");
    TS_ASSERT_EQUALS((*fun).type(),"TimesFunction");
    delete fun;

  }

  void testScalarDeclaration()
  {
    Variable* var = VariableFactory::instance().create("Scalar");
    TS_ASSERT(var);
    delete var;
  }
};

#endif // SCALARTEST_H
