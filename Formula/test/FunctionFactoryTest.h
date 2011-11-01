#ifndef FUNCTIONFACTORYTEST_H
#define FUNCTIONFACTORYTEST_H

#include "../FunctionFactory.h"
#include "TestSuite.h"
#include "DummyFun.h"

#include <iostream>

using namespace Formula;

class FunctionFactoryTest
{
//------------ local defines -----------------------------------------


//--------------------------------------------------------------------
public:
  static void test()
  {
    FunctionFactoryTest p;
    p.testDummy();
  }

  FunctionFactoryTest()
  {
    FunctionFactory::instance().subscribeFunction<AScalarFunction>("test+","Scalar,...");
  }

  void testDummy()
  {
    std::vector<Argument> args;
    for(int i=0;i<3;++i)
    {
      Scalar* v = new Scalar;
      v->setValue((i+1)*2);
      args.push_back(Argument(v));
    }
    Function* fun = FunctionFactory::instance().create("test+",args);
    TS_ASSERT(fun);
    fun->setArguments(args);
    TS_ASSERT_EQUALS(fun->eval().toString(),"12");
    TS_ASSERT_EQUALS((*fun)().type(),"Scalar");
    delete fun;
  }

};

#endif // FUNCTIONFACTORYTEST_H
