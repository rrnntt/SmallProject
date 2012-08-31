#include "gtest/gtest.h"
#include "Numeric/ScaledChebfun.h"
#include "Numeric/Constants.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <iostream>

using namespace Numeric;

/*========================================================================*/
class TestScaledChebfun: public ScaledChebfun
{
public:
  TestScaledChebfun(size_t n, const double& startX, const double& endX):
  ScaledChebfun(n, startX, endX)
  {}
  using ScaledChebfun::transform;
};

namespace // anonymous namespace
{

double gauss(double x)
{
  return exp(-x*x);
}

double dgauss(double x)
{
  return -2*x*exp(-x*x);
}

double xgauss(double x)
{
  if ( fabs( x ) == inf ) return 0.0;
  const double t = x - 5.0;
  return x*exp(-0.1*t*t);
}

class TestScaledChebfun_Gauss: public virtual IFunction1D, public virtual ParamFunction
{
public:
  TestScaledChebfun_Gauss()
  {
  }
  virtual std::string name()const {return "TestScaledChebfun_Gauss";}
protected:
  virtual void function1D(double* out, const double* xValues, const size_t nData)const
  {
    for(size_t i = 0; i < nData; ++i)
    {
      double x = xValues[i];
      out[i] = exp(-x*x);
    }
  }
  virtual void functionDeriv1D(Jacobian* , const double* , const size_t )
  {
  }
};

} // anonymous namespace

/*========================================================================*/

TEST(Numeric_ScaledChebfun_Test, InfinityTest)
{
  double plusInf = inf;
  EXPECT_TRUE( plusInf == inf );
  EXPECT_TRUE( plusInf != minf );
  EXPECT_TRUE( plusInf != 10.0 );

  double minusInf = minf;
  EXPECT_TRUE( minusInf != inf );
  EXPECT_TRUE( minusInf == minf );
  EXPECT_TRUE( minusInf != 10.0 );

  double absInf = fabs( minusInf );
  EXPECT_TRUE( absInf == inf );
  EXPECT_TRUE( absInf != minf );
  EXPECT_TRUE( absInf != 10.0 );

}

TEST(Numeric_ScaledChebfun_Test, CreationTest)
{
  // bounds cannot be the same
  EXPECT_THROW( ScaledChebfun a(10, 1, 1), std::invalid_argument );
  // left bound cannot be greater than right bound
  EXPECT_THROW( ScaledChebfun a(10, 1, -1), std::invalid_argument );
  EXPECT_THROW( ScaledChebfun a(10, inf, 1), std::invalid_argument );
  EXPECT_THROW( ScaledChebfun a(10, 2, minf), std::invalid_argument );
  // all real axis isn't allowed
  EXPECT_THROW( ScaledChebfun a(10, minf, inf), std::invalid_argument );
  EXPECT_THROW( ScaledChebfun a(10, inf, minf), std::invalid_argument );
  EXPECT_THROW( ScaledChebfun a(10, minf, minf), std::invalid_argument );
  EXPECT_THROW( ScaledChebfun a(10, inf, inf), std::invalid_argument );

  // valid arguments
  EXPECT_NO_THROW( ScaledChebfun a(10, minf, 4) );
  EXPECT_NO_THROW( ScaledChebfun a(10, 3, inf) );
  EXPECT_NO_THROW( ScaledChebfun a(10, 3.789, 4.124) );

}

TEST(Numeric_ScaledChebfun_Test, ScalingTest)
{
  ScaledChebfun a(10, minf, 4);
  EXPECT_TRUE( a.hasScaling() );

  ScaledChebfun b(10, 3, inf);
  EXPECT_TRUE( b.hasScaling() );

  ScaledChebfun c(10, 3.789, 4.124);
  EXPECT_FALSE( c.hasScaling() );

}

TEST(Numeric_ScaledChebfun_Test, ConstructorTest)
{
  ScaledChebfun f;
  EXPECT_EQ( f.n(), 0 );
  EXPECT_EQ( f.startX(), -1 );
  EXPECT_EQ( f.endX(), 1 );
}

TEST(Numeric_ScaledChebfun_Test, CopyConstructorTest)
{
  ScaledChebfun one(10, 7, inf);
  ScaledChebfun two( one );
  EXPECT_TRUE( two.haveSameBase( one ) );
}

TEST(Numeric_ScaledChebfun_Test, TransformTest)
{
  TestScaledChebfun fun(10, -2, 5);
  EXPECT_EQ( fun.transform( 3 ), 3 );
  EXPECT_EQ( fun.transform( -2 ), -2 );
  EXPECT_EQ( fun.transform( 5 ), 5 );

  TestScaledChebfun fun_minf(10, minf, 5);
  EXPECT_EQ( fun_minf.transform( minf ), -1 );
  EXPECT_NEAR( fun_minf.transform( 3 ), -0.333333, 1e-6 );
  EXPECT_EQ( fun_minf.transform( 5 ), 1 );

  TestScaledChebfun fun_inf(10, -2, inf);
  EXPECT_EQ( fun_inf.transform( -2 ), -1 );
  EXPECT_NEAR( fun_inf.transform( 3 ), 0.666666, 1e-6 );
  EXPECT_EQ( fun_inf.transform( inf ), 1 );

}

TEST(Numeric_ScaledChebfun_Test, FitAFunctionTest)
{
  ScaledChebfun a(15, 0, 2*pi);
  a.fit( sin );
  FunctionDomain1DVector x(0, 2*pi, 10);
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << sin( x[i] ) - a.value( x[i] ) << std::endl;
    EXPECT_NEAR(  a.value( x[i] ), sin( x[i] ), 1e-10 );
  }

  ScaledChebfun b(100, 0, inf);
  b.fit( gauss );
  FunctionDomain1DVector x1(0, 100, 10);
  for(size_t i = 0; i < x1.size(); ++i)
  {
    //std::cerr << x1[i] << ' ' << gauss( x1[i] ) - b.value( x1[i] ) << std::endl;
    EXPECT_NEAR(  b.value( x1[i] ), gauss( x1[i] ), 1e-14 );
  }
  FunctionDomain1DVector x2(0, 1, 10);
  for(size_t i = 0; i < x2.size(); ++i)
  {
    //std::cerr << x2[i] << ' ' << gauss( x2[i] ) - b.value( x2[i] ) << std::endl;
    EXPECT_NEAR(  b.value( x2[i] ), gauss( x2[i] ), 1e-14 );
  }

  ScaledChebfun c(100, minf, 0);
  c.fit( gauss );
  FunctionDomain1DVector x3(-100, 0, 10);
  for(size_t i = 0; i < x3.size(); ++i)
  {
    //std::cerr << x3[i] << ' ' << gauss( x3[i] ) - c.value( x3[i] ) << std::endl;
    EXPECT_NEAR(  c.value( x3[i] ), gauss( x3[i] ), 1e-14 );
  }
  FunctionDomain1DVector x4(-1, 0, 10);
  for(size_t i = 0; i < x4.size(); ++i)
  {
    //std::cerr << x4[i] << ' ' << gauss( x4[i] ) - c.value( x4[i] ) << std::endl;
    EXPECT_NEAR(  c.value( x4[i] ), gauss( x4[i] ), 1e-14 );
  }

  ScaledChebfun d(220, 0, inf);
  d.fit( xgauss );
  FunctionDomain1DVector x5(0, 30, 20);
  for(size_t i = 0; i < x5.size(); ++i)
  {
    double xx = x5[i];
    std::cerr << xx << ' ' << xgauss( xx )  - d.value( xx ) << std::endl;
    //EXPECT_NEAR(  d.value( xx ), xgauss( xx ), 1e-14 );
  }
  FunctionDomain1DVector x6(0, 1, 10);
  for(size_t i = 0; i < x6.size(); ++i)
  {
    double xx = x6[i];
    std::cerr << xx << ' ' << xgauss( xx )  - d.value( xx ) << std::endl;
    //EXPECT_NEAR(  d.value( xx ), xgauss( xx ), 1e-14 );
  }
  FunctionDomain1DVector x7(30, 100, 10);
  for(size_t i = 0; i < x7.size(); ++i)
  {
    double xx = x7[i];
    std::cerr << xx << ' ' << xgauss( xx ) - d.value( xx ) << std::endl;
    //EXPECT_NEAR(  d.value( xx ), xgauss( xx ), 1e-14 );
  }

}

TEST(Numeric_ScaledChebfun_Test, FitIFunction1DTest)
{
  TestScaledChebfun_Gauss Gauss;
  ScaledChebfun b(100, 0, inf);
  b.fit( Gauss );
  FunctionDomain1DVector x1(0, 100, 10);
  FunctionValues y1( x1 );
  Gauss.function(x1, y1);
  for(size_t i = 0; i < x1.size(); ++i)
  {
    //std::cerr << x1[i] << ' ' << y1.getCalculated(i) - b.value( x1[i] ) << std::endl;
    EXPECT_NEAR(  b.value( x1[i] ), y1.getCalculated(i), 1e-14 );
  }

  FunctionDomain1DVector x2(0, 2, 10);
  FunctionValues y2( x2 );
  Gauss.function(x2, y2);
  for(size_t i = 0; i < x2.size(); ++i)
  {
    //std::cerr << x2[i] << ' ' << y2.getCalculated(i) << ' ' << y2.getCalculated(i) - b.value( x2[i] ) << std::endl;
    EXPECT_NEAR(  b.value( x2[i] ), y2.getCalculated(i), 1e-14 );
  }

}

TEST(Numeric_ScaledChebfun_Test, FromDerivativeTest)
{
  // positive infinity
  ScaledChebfun ap(100, 0, inf);
  ap.fit( gauss );

  ScaledChebfun dp;
  dp.fromDerivative( ap );
  FunctionDomain1DVector x1(0, 1, 10);
  FunctionDomain1DVector x100(0, 100, 10);

  for(size_t i = 0; i < x1.size(); ++i)
  {
    const double x = x1[i];
    //std::cerr << x << ' ' << dgauss( x ) << ' ' << dp.value( x ) << std::endl;
    EXPECT_NEAR(  dp.value( x ), dgauss( x ), 1e-11 );
  }

  for(size_t i = 0; i < x100.size(); ++i)
  {
    const double x = x100[i];
    //std::cerr << x << ' ' << dgauss( x ) << ' ' << dp.value( x ) << std::endl;
    EXPECT_NEAR(  dp.value( x ), dgauss( x ), 1e-11 );
  }

  // negative infinity
  ScaledChebfun am(100, minf, 0);
  am.fit( gauss );

  ScaledChebfun dm;
  dm.fromDerivative( am );
  FunctionDomain1DVector xm1(-1, 0, 10);
  FunctionDomain1DVector xm100(-100, 0, 10);

  for(size_t i = 0; i < xm1.size(); ++i)
  {
    const double x = xm1[i];
    //std::cerr << x << ' ' << dgauss( x ) << ' ' << dm.value( x ) << std::endl;
    EXPECT_NEAR(  dm.value( x ), dgauss( x ), 1e-11 );
  }

  for(size_t i = 0; i < xm100.size(); ++i)
  {
    const double x = xm100[i];
    //std::cerr << x << ' ' << dgauss( x ) << ' ' << dm.value( x ) << std::endl;
    EXPECT_NEAR(  dm.value( x ), dgauss( x ), 1e-11 );
  }
}

TEST(Numeric_ScaledChebfun_Test, IntegrTest)
{
  // positive infinity
  ScaledChebfun ap(100, 0, inf);
  ap.fit( gauss );
  //std::cerr << ap.integr() - sqrt(pi)/2 << std::endl;
  EXPECT_NEAR( ap.integr(), sqrt(pi)/2, 1e-15);

  // negative infinity
  ScaledChebfun am(100, minf, 0);
  am.fit( gauss );
  //std::cerr << am.integr() - sqrt(pi)/2 << std::endl;
  EXPECT_NEAR( am.integr(), sqrt(pi)/2, 1e-15);
}

TEST(Numeric_ScaledChebfun_Test, BestFitTest)
{
  // positive infinity
  ScaledChebfun ap(0, 0, inf);
  ap.bestFit( gauss );
  //std::cerr << "n=" << ap.n() << std::endl;
  EXPECT_EQ( ap.n(), 129 );
}
