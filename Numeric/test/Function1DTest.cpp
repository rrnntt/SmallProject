#include "gtest/gtest.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <iostream>

using namespace Numeric;

class Function1DTestFunction: public virtual IFunction1D, public virtual ParamFunction
{
public:
  Function1DTestFunction()
  {
    declareParameter("A",1.0);
    declareParameter("B",-3.0);
  }
  virtual std::string name()const {return "Function1DTestFunction";}
protected:
  virtual void function1D(double* out, const double* xValues, const size_t nData)const
  {
    double A = getParameter(0);
    double B = getParameter(1);
    for(size_t i = 0; i < nData; ++i)
    {
      double x = xValues[i];
      out[i] = A*x+B;
    }
  }
  virtual void functionDeriv1D(Jacobian* jacobian, const double* xValues, const size_t nData)
  {
    double A = getParameter(0);
    double B = getParameter(1);
    for(size_t i = 0; i < nData; ++i)
    {
      double x = xValues[i];
      jacobian->set(i,0,x);
      jacobian->set(i,1,1.0);
    }
  }
};

TEST(Function1DTest, Test)
{
  Function1DTestFunction fun;
  FunctionDomain1DVector domain(0.0,1.0,10);
  FunctionValues values(domain);
  fun.function(domain,values);
  for(size_t i = 0; i < domain.size(); ++i)
  {
    double x = domain[i];
    EXPECT_EQ( 1.0 * x - 3.0, values.getCalculated(i));
  }
}

