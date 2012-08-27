#include "gtest/gtest.h"
#include "Numeric/ScaledChebfun.h"
#include "Numeric/Constants.h"

#include <iostream>

using namespace Numeric;

class TestScaledChebfun: public ScaledChebfun
{
public:
  TestScaledChebfun(size_t n, const double& startX, const double& endX):
  ScaledChebfun(n, startX, endX)
  {}
  using ScaledChebfun::transform;
};

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
