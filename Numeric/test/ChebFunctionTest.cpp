#include "gtest/gtest.h"
#include "Numeric/ChebFunction.h"
#include "Numeric/FunctionDomain1D.h"

using namespace Numeric;

double ChebFunction_gauss(double x)
{
  return exp(-x*x);
}

TEST(Numeric_ChebFunction_Test, Test)
{
  ChebFunction fun(10, minf, -4);
  fun.appendRight(50, 4);
  fun.appendRight(10, inf);

  fun.fit( ChebFunction_gauss );

  FunctionDomain1DVector x(0,20,100);
  FunctionValues y( x );
  fun.eval( x, y );

  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = x[i];
    double yy = y.getCalculated(i);
    //std::cerr << xx << ' ' << yy - ChebFunction_gauss( xx ) << std::endl;
    EXPECT_NEAR( yy, ChebFunction_gauss( xx ), 1e-11 );
  }

  //std::cerr << "integr " << fun.integr() << ' ' << sqrt(pi) << std::endl;
  EXPECT_NEAR( fun.integr(), sqrt(pi), 1e-13 );
}

TEST(Numeric_ChebFunction_Test, DerivativeTest)
{
  ChebFunction fun(12, minf, -4);
  fun.appendRight(50, 4);
  fun.appendRight(12, inf);

  fun.fit( ChebFunction_gauss );

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
    EXPECT_NEAR( yy, -2*xx*ChebFunction_gauss( xx ), 1e-10 );
  }

  //std::cerr << "integr " << dfun.integr() << std::endl;
  EXPECT_NEAR( dfun.integr(), 0, 1e-13 );
}
