#include "gtest/gtest.h"
#include "Formula/Variable.h"

#include <iostream>
#include <sstream>

using namespace Formula;

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

TEST(VariableTest, Dummy) 
{
    DummyVar var;
    EXPECT_EQ(var.type(),"DummyVar");
    var.stuff = 25;
    EXPECT_EQ(var.stuff,25);
    const DummyVar& dvar = dynamic_cast<const DummyVar&>(var());
    EXPECT_EQ(var.stuff,dvar.stuff);
}

TEST(VariableTest, Equal) 
{
    DummyVar v1,v2;
    v1.stuff = 12;
    v2.stuff = 12;
    bool eq = v1 == v2;
    EXPECT_TRUE(eq);
    v2.stuff = 11;
    eq = v1 != v2;
    EXPECT_TRUE(eq);
}

