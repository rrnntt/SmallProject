#ifndef SINGLETONHOLDERTEST_H
#define SINGLETONHOLDERTEST_H

#include "../SingletonHolder.h"
#include "TestSuite.h"

#include <iostream>

using namespace Kernel;

class Class
{
public:
  Class(){std::cerr<<"Created\n";}
  ~Class(){std::cerr<<"Destroyed\n";}
  int field;
};

typedef SingletonHolder<Class> SingletonClass;

class SingletonHolderTest
{
public:
  static void test()
  {
    SingletonHolderTest test;
    test.testSingleton();
    test.testSingleton1();
  }

  void testSingleton()
  {
    SingletonClass::instance().field = 12;
    TS_ASSERT_EQUALS(SingletonClass::instance().field,12);
    std::cerr<<"Test1\n";
  }
  void testSingleton1()
  {
    TS_ASSERT_EQUALS(SingletonClass::instance().field,12);
    std::cerr<<"Test2\n";
  }
};

#endif // SINGLETONHOLDERTEST_H
