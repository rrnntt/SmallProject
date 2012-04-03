#include "gtest/gtest.h"
#include "Numeric/ParamFunction.h"

#include <iostream>

using namespace Numeric;

class ParamFunctionTestFunction: public ParamFunction
{
public:
  ParamFunctionTestFunction()
  {
    declareParameter("A");
    declareParameter("B");
    declareParameter("C");
  }
  /// Returns the function's name
  virtual std::string name()const {return "ParamFunctionTestFunction";}
  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(const FunctionDomain&, FunctionValues&)const {}
};

TEST(ParamFunctionTest, Test)
{
  ParamFunctionTestFunction fun;
  fun.setParameter(0,100);
  fun.setParameter(1,200);
  fun.setParameter(2,300);
  EXPECT_EQ(fun.nParams(),3);
  EXPECT_EQ(fun.parameterName(0),"A");
  EXPECT_EQ(fun.parameterName(1),"B");
  EXPECT_EQ(fun.parameterName(2),"C");
  EXPECT_EQ(fun.getParameter(0),100);
  EXPECT_EQ(fun.getParameter(1),200);
  EXPECT_EQ(fun.getParameter(2),300);
  fun.fix(1);
  fun.fix(2);
  EXPECT_EQ(fun.nameOfActive(0),"A");
  EXPECT_EQ(fun.activeParameter(0),100);
  fun.unfix(2);
  EXPECT_EQ(fun.nameOfActive(1),"C");
  EXPECT_EQ(fun.activeParameter(1),300);
  fun.unfix(1);
  EXPECT_EQ(fun.nameOfActive(1),"B");
  EXPECT_EQ(fun.activeParameter(1),200);
}

