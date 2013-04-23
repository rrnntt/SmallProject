#ifndef TESTPARSER_H
#define TESTPARSER_H

#include "../DynamicFactory.h"
#include "TestSuite.h"

#include <iostream>

using namespace Kernel;

class ABase
{
public:
  ABase():m_var(11){}
  int m_var;
};

class AClass: public ABase
{
public:
  AClass(){m_var = 22;}
};

class BClass: public ABase
{
public:
  BClass(){m_var = 33;}
};

class Factory: public DynamicFactory<ABase>
{
public:
  Factory(){}
};

class TestDynamicFactory
{
public:
  static void test()
  {
    TestDynamicFactory test;
    test.testAFactory();
  }

  void testAFactory()
  {

    Factory fac;
    fac.subscribe<AClass>("AClass");
    fac.subscribe<BClass>("BClass");
    ABase* a = fac.create("AClass");
    TS_ASSERT(a);
    TS_ASSERT_EQUALS(a->m_var,22);
    ABase* b = fac.create("BClass");
    TS_ASSERT(b);
    TS_ASSERT_EQUALS(b->m_var,33);
  }

};

#endif // TESTPARSER_H
