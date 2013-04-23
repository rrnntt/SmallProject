#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include "../Function.h"
#include "../Scalar.h"
#include "TestSuite.h"

#include <iostream>
#include <sstream>

using namespace Formula;

class FunctionTest
{

  class DummyVar: public Variable
  {
  public:
    virtual std::string type()const{return "DummyVar";}
    virtual std::string toString()const
    {
      std::ostringstream ostr;
      ostr << type() << ' ' << stuff;
      return ostr.str();
    }
    virtual bool setValue(const std::string&){return false;}
    virtual bool setValue(const Variable&){return false;}
    virtual bool setValue(const double*){return false;}
    virtual Variable* clone()const{return NULL;}
    bool operator==(const Variable& v)
    {
      return stuff == v.as<DummyVar>().stuff;
    }

    int stuff;
  };

  class AFunction: public ReturnTypeFunction<Scalar>
  {
  public:
    const Variable& eval()const
    {
      m_return.setValue(0.0);
      for(size_t i=0;i<m_argv.size();++i)
      {
        Scalar tmp(m_argv[i]().as<DummyVar>().stuff);
        m_return.plus(tmp);
      }
      return m_return;
    }
    int testArguments(std::vector<Argument>& args)
    {
      for(size_t i=0;i<args.size();++i)
      {
        if (args[i]().type() != "DummyVar") return i+1;
      }
      return 0;
    }
  };

public:
  static void test()
  {
    FunctionTest p;
    p.testDummy();
  }

  void testDummy()
  {
    AFunction f;
    std::vector<Argument> args;
    for(int i=0;i<3;++i)
    {
      DummyVar* v = new DummyVar;
      v->stuff = i+1;
      args.push_back(Argument(v));
    }
    TS_ASSERT_EQUALS(f.testArguments(args),0);
    std::vector<Argument> bad_args(args);
    bad_args.push_back(Argument(new Scalar));
    TS_ASSERT_EQUALS(f.testArguments(bad_args),4);

    f.setArguments(args);
    TS_ASSERT_EQUALS(f.eval().toString(),"6");
  }
};

#endif // FUNCTIONTEST_H
