#include "gtest/gtest.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/UserFunction1D.h"

using namespace Numeric;

namespace // anonymous
{
  double gauss(double x)
  {
    return exp(-x*x);
  }


}

//====================================================================
TEST(Numeric_ChebFunction_Test, Test)
{
  ChebFunction fun(10, minf, -4);
  fun.appendRight(50, 4);
  fun.appendRight(10, inf);

  fun.fit( gauss );

  FunctionDomain1DVector x(0,20,100);
  FunctionValues y( x );
  fun.eval( x, y );

  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = x[i];
    double yy = y.getCalculated(i);
    //std::cerr << xx << ' ' << yy - ChebFunction_gauss( xx ) << std::endl;
    EXPECT_NEAR( yy, gauss( xx ), 1e-11 );
  }

  //std::cerr << "integr " << fun.integr() << ' ' << sqrt(pi) << std::endl;
  EXPECT_NEAR( fun.integr(), sqrt(pi), 1e-13 );
}

//====================================================================
TEST(Numeric_ChebFunction_Test, DerivativeTest)
{
  ChebFunction fun(12, minf, -4);
  fun.appendRight(50, 4);
  fun.appendRight(12, inf);

  fun.fit( gauss );

  ChebFunction dfun;
  dfun.fromDerivative( fun );

  FunctionDomain1DVector x(0,20,100);
  FunctionValues y( x );
  dfun.eval( x, y );

  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = x[i];
    double yy = y.getCalculated(i);
    //std::cerr << xx << ' ' << yy -  -2*xx*ChebFunction_gauss( xx ) << std::endl;
    EXPECT_NEAR( yy, -2*xx*gauss( xx ), 1e-10 );
  }

  //std::cerr << "integr " << dfun.integr() << std::endl;
  EXPECT_NEAR( dfun.integr(), 0, 1e-13 );
}

//====================================================================
TEST(Numeric_ChebFunction_Test, BestFitTest)
{
  UserFunction1D xSquared;
  xSquared.setAttributeValue("Formula","x^2");

  ChebFunction f2(0, -1, 1);
  f2.bestFit( xSquared );
  auto& a = f2.cfun(0).coeffs();
  for(size_t i = 0; i < a.size(); ++i)
  {
    std::cerr << i << ' ' << a[i] << std::endl;
  }
}
