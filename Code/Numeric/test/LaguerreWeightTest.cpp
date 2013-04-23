#include "gtest/gtest.h"
#include "Numeric/LaguerreWeight.h"
#include "Numeric/Laguerre.h"
#include "Numeric/Chebfun.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/StdJacobian.h"

using namespace Numeric;

void testLaguerreWeight(double alpha, double range = 100.0)
{
  LaguerreWeight lagW;
  lagW.setParameter("Alpha", alpha);
  chebfun cf(0,0.0,range);
  cf.bestFit( lagW );

  Laguerre lagP(alpha,1);
  const double int1 = lagP.weightIntegral();
  const double int2 = cf.integr();
  EXPECT_NEAR(int1, int2, 1e-9);
  //std::cerr << int1 << ' ' << int2 << ' ' << int1 - int2 << std::endl;
}

void testLaguerreWeightDerivs(double alpha, const double tol = 1e-6)
{
  LaguerreWeight lagW;
  lagW.setParameter("Alpha", alpha);

  const size_t n = 5;
  const double start = alpha == 0.0 ? 0.0001 : 0.0;
  FunctionDomain1DVector d(start, 10, n);
  FunctionValues v0(d);
  lagW.function(d, v0);

  const double da = 0.000001;
  // make a shifted in alpha chebfun
  lagW.setParameter("Alpha", alpha + da);
  FunctionValues v1(d);
  lagW.function(d, v1);

  lagW.setParameter("Alpha", alpha);

  StdJacobian jacobian(n,1);
  lagW.functionDeriv(d, jacobian);

  for(size_t i = 0; i < n; ++i)
  {
    const double dLagW = jacobian.get(i, 0);
    const double dAlpha = (v1.getCalculated(i) - v0.getCalculated(i))/da;
    EXPECT_NEAR( dLagW, dAlpha, tol );
    //std::cerr << i << ":  " << d[i] << ' ' << v0.getCalculated(i) << std::endl;
    //std::cerr << "    " << dLagW - dAlpha << std::endl;
  }
}

TEST(Numeric_LaguerreWeight_Test, Test0)
{
  testLaguerreWeight(0);
}

TEST(Numeric_LaguerreWeight_Test, Test1)
{
  testLaguerreWeight(1);
}

TEST(Numeric_LaguerreWeight_Test, Test2)
{
  testLaguerreWeight(2);
}

TEST(Numeric_LaguerreWeight_Test, Test5)
{
  testLaguerreWeight(5);
}

TEST(Numeric_LaguerreWeight_Test, Test8)
{
  testLaguerreWeight(8);
}

TEST(Numeric_LaguerreWeight_Test, TestNonInt)
{
  testLaguerreWeight(4.15, 40.0);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivs0)
{
  testLaguerreWeightDerivs(0, 1e-4);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivs1)
{
  testLaguerreWeightDerivs(1);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivs2)
{
  testLaguerreWeightDerivs(2);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivs5)
{
  testLaguerreWeightDerivs(5, 1e-4);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivs8)
{
  testLaguerreWeightDerivs(8, 0.02);
}

TEST(Numeric_LaguerreWeight_Test, TestDerivsNonInt)
{
  testLaguerreWeightDerivs(4.15, 1e-5);
}

