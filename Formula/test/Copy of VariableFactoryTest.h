#ifndef VARIABLEFACTORYTEST_H
#define VARIABLEFACTORYTEST_H

#include "../VariableFactory.h"
#include "TestSuite.h"

#include <iostream>

using namespace Formula;

class DummyVar: public Variable
{
public:
  virtual std::string type()const{return "DummyVar";}
  virtual std::string toString()const{return "OK";}
  virtual bool setValue(const std::string&){return false;}
  virtual bool setValue(const Variable&){return false;}
  virtual Variable* clone()const{return NULL;}
  int stuff;
};

DECLARE_VARIABLE(DummyVar)

class VariableFactoryTest
{
public:
  static void test()
  {
    VariableFactoryTest p;
    p.testDummy();
  }

  void testDummy()
  {
    Variable* var = Formula::VariableFactory::instance().create("DummyVar");
    TS_ASSERT(var);
    TS_ASSERT_EQUALS(var->type(),"DummyVar");
    ((DummyVar*)var)->stuff = 25;
    TS_ASSERT_EQUALS(((DummyVar*)var)->stuff,25);
    const DummyVar& dvar = dynamic_cast<const DummyVar&>((*var)());
    TS_ASSERT_EQUALS(((DummyVar*)var)->stuff,dvar.stuff);
  }
};

#endif // VARIABLEFACTORYTEST_H
