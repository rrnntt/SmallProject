#include "gtest/gtest.h"
#include "Numeric/Function1D.h"
#include "Numeric/ParamFunction.h"

#include <iostream>

using namespace Numeric;

class Function1DTestFunction: public virtual Function1D, public virtual ParamFunction
{
public:
  Function1DTestFunction()
  {
    declareParameter("A",1.0);
    declareParameter("B",-3.0);
  }
  virtual std::string name()const {return "Function1DTestFunction";}
protected:
  virtual void function1D(FunctionDomain1D& domain)const
  {
    double A = getParameter(0);
    double B = getParameter(1);
    for(size_t i = 0; i < domain.size(); ++i)
    {
      double x = domain.getX(i);
      domain.setCalculated(i,A*x+B);
    }
  }
  virtual void functionDeriv1D(FunctionDomain1D& domain, Jacobian& jacobian)
  {
    double A = getParameter(0);
    double B = getParameter(1);
    for(size_t i = 0; i < domain.size(); ++i)
    {
      double x = domain.getX(i);
      jacobian.set(i,0,x);
      jacobian.set(i,1,1.0);
    }
  }
};

TEST(Function1DTest, Test)
{
  Function1DTestFunction fun;
  FunctionDomain1D domain(0.0,1.0,10);
  fun.function(domain);
  for(size_t i = 0; i < domain.size(); ++i)
  {
    double x = domain.getX(i);
    EXPECT_EQ( 1.0 * x - 3.0, domain.getCalculated(i));
  }
}

