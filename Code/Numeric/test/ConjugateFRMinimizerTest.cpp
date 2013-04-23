#include "gtest/gtest.h"
#include "Numeric/ConjugateFRMinimizer.h"
#include "Numeric/CostFunction.h"

#include <iostream>

using namespace Numeric;

class ConjugateFRMinimizerTestCostFunction: public CostFunction
{
public:
  ConjugateFRMinimizerTestCostFunction():A(),B(){}
  /// Get i-th parameter
  virtual double getParameter(size_t i)const
  {
    if (i == 0)
    {
      return A;
    }
    return B;
  }
  /// Set i-th parameter
  virtual void setParameter(size_t i, const double& value)
  {
    if (i == 0) A = value;
    else
      B = value;
  }
  /// Total number of parameters
  virtual size_t nParams()const {return 2;}
  /// Evaluate the function
  virtual double eval() const 
  {
    double x = A - 1.0;
    double y = B - 2.1;
    return 3.0 + x*x + y*y;
  }
  /// Evaluate the derivatives
  virtual void deriv(std::vector<double>& der) const 
  {
    size_t np = this->nParams();
    if (der.size() != np)
    {
      der.resize(np);
    }
    double x = A - 1.0;
    double y = B - 2.1;
    der[0] = 2*x;
    der[1] = 2*y;
  }
  /// Evaluate the function and the derivatives
  virtual double evalAndDeriv(std::vector<double>& der) const 
  {
    deriv(der);
    return eval();
  }
private:
  double A,B;
};

TEST(ConjugateFRMinimizerTest, Test)
{
  CostFunction_ptr fun(new ConjugateFRMinimizerTestCostFunction);
  ConjugateFRMinimizer mzr;
  mzr.setCostFunction(fun);
  mzr.minimize();
  std::cerr << "ConjugateFRMinimizer " << fun->eval() << std::endl;
  std::cerr << "A=" << fun->getParameter(0) << std::endl;
  std::cerr << "B=" << fun->getParameter(1) << std::endl;
}

