#include "gtest/gtest.h"
#include "Numeric/JacobiWeight.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/Chebfun.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/StdJacobian.h"

using namespace Numeric;

void testJacobiWeight(double alpha, double beta)
{
  JacobiWeight jacW;
  jacW.setParameter("Alpha", alpha);
  jacW.setParameter("Beta", beta);
  chebfun cf;
  cf.bestFit( jacW );

  JacobiPolynomial jacP(alpha,beta,1);
  EXPECT_NEAR(jacP.weightIntegral(), cf.integr(), 1e-12);
}

void testJacobiWeightDerivs(double alpha, double beta, const double tol = 1e-16)
{
  JacobiWeight jacW;
  jacW.setParameter("Alpha", alpha);
  jacW.setParameter("Beta", beta);
  chebfun cf;
  cf.bestFit( jacW, tol );

  const double da = 0.000001;
  // make a shifted in alpha chebfun
  jacW.setParameter("Alpha", alpha + da);
  chebfun cfa;
  cfa.bestFit( jacW, tol );

  // make a shifted in beta chebfun
  jacW.setParameter("Alpha", alpha);
  jacW.setParameter("Beta", beta + da);
  chebfun cfb;
  cfb.bestFit( jacW, tol );

  jacW.setParameter("Alpha", alpha);
  jacW.setParameter("Beta", beta);

  const size_t n = 5;
  FunctionDomain1DVector d(-1,1, n);
  StdJacobian jacobian(n,2);
  jacW.functionDeriv(d, jacobian);

  for(size_t i = 0; i < n; ++i)
  {
    const double x = d[i];
    const double dAlpha = (cfa(x) - cf(x))/da;
    const double dBeta = (cfb(x) - cf(x))/da;
    EXPECT_NEAR( jacobian.get(i, 0), dAlpha, 1e-6 );
    EXPECT_NEAR( jacobian.get(i, 1), dBeta, 1e-6 );
    //std::cerr << i << ':' << std::endl;
    //std::cerr << "    " << jacobian.get(i, 0) - dAlpha << std::endl;
    //std::cerr << "    " << jacobian.get(i, 1) - dBeta << std::endl;
  }
}

TEST(Numeric_JacobiWeight_Test, Test11)
{
  testJacobiWeight(1.0, 1.0);
}

TEST(Numeric_JacobiWeight_Test, Test12)
{
  testJacobiWeight(1.0, 2.0);
}

TEST(Numeric_JacobiWeight_Test, Test22)
{
  testJacobiWeight(2.0, 2.0);
}

TEST(Numeric_JacobiWeight_Test, Test02)
{
  testJacobiWeight(0.0, 2.0);
}

TEST(Numeric_JacobiWeight_Test, Test20)
{
  testJacobiWeight(2.0, 0.0);
}

TEST(Numeric_JacobiWeight_Test, TestNonInt)
{
  testJacobiWeight(2.3, 3.2);
}

TEST(Numeric_JacobiWeight_Test, TestDerivs11)
{
  testJacobiWeightDerivs(1.0, 1.0, 1e-6);
}

TEST(Numeric_JacobiWeight_Test, TestDerivs12)
{
  testJacobiWeightDerivs(1.0, 2.0);
}

