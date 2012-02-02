#include "gtest/gtest.h"
#include "Numeric/SimplexMinimizer.h"

#include <iostream>
#include <vector>

using namespace Numeric;

class DummyCostFunction: public CostFunction
{
public:
  DummyCostFunction():A(0),B(0){}
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
  virtual void deriv(std::vector<double>&) const {}
private:
  double A,B;
};

TEST(SimplexMinimizerTest, Test)
{
  CostFunction_ptr fun(new DummyCostFunction);
  SimplexMinimizer simplex;
  simplex.setCostFunction(fun);
  simplex.minimize();
  std::cerr << "SimplexMinimizer " << fun->eval() << std::endl;
  std::cerr << "A=" << fun->getParameter(0) << std::endl;
  std::cerr << "B=" << fun->getParameter(1) << std::endl;
}

