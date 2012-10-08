#include "gtest/gtest.h"
#include "Numeric/CustomPolynomial.h"
#include "Numeric/Laguerre.h"
#include "Numeric/LaguerreWeight.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/Constants.h"

using namespace Numeric;

TEST(Numeric_CustomPolynomial_Test, LaguerreTest)
{
  Laguerre lagP(0.0, 10);
  CustomPolynomial cp( 10, 0.0, 120.0 );
  cp.setWeightFunction( lagP.weightFunction() );

  auto& a = cp.getA();
  auto& al = lagP.getA();

  EXPECT_EQ( a.size(), 10 );

  double sum = 0;
  auto& w = cp.getWeights();
  for(size_t i = 0; i < a.size(); ++i)
  {
    //std::cerr << i << ' ' << a[i] << ' ' << al[i] << ' ' << fabs(a[i]) - fabs(al[i]) << std::endl;
    EXPECT_NEAR( fabs(a[i]) - fabs(al[i]), 0.0, 1e-13 );
    sum += w[i];
  }
  //std::cerr << "Weight " << sum << ' ' << lagP.weightIntegral() << ' ' << cp.weightIntegral() << std::endl;
  EXPECT_NEAR( sum, lagP.weightIntegral(), 1e-9 );
  EXPECT_NEAR( sum, cp.weightIntegral(), 1e-9 );
}

TEST(Numeric_CustomPolynomial_Test, JacobiTest)
{
  JacobiPolynomial P(1.0, 1.0, 10);
  CustomPolynomial cp( 10, -1.0, 1.0 );
  cp.setWeightFunction( P.weightFunction() );

  auto& rP = P.getRoots();
  auto& r = cp.getRoots();

  auto& a = cp.getB();
  auto& ap = P.getB();

  EXPECT_EQ( a.size(), 10 );

  double sum = 0;
  auto& w = cp.getWeights();
  for(size_t i = 0; i < a.size(); ++i)
  {
    std::cerr << i <<": "<<std::endl;
    std::cerr << "      " << a[i] << ' ' << ap[i] << ' ' << fabs(a[i]) - fabs(ap[i]) << std::endl;
    std::cerr << "      " << r[i]<< ' ' << rP[i] << std::endl;
    //EXPECT_NEAR( fabs(a[i]) - fabs(ap[i]), 0.0, 1e-13 );
    sum += w[i];
  }
  std::cerr << "Weight " << sum << ' ' << P.weightIntegral() << ' ' << cp.weightIntegral() << std::endl;
}

TEST(Numeric_CustomPolynomial_Test, ChebyshevTest)
{
  system("pause");
  auto user = FunctionFactory::instance().createFitFunction("UserFunction1D(Formula=sqrt(1-x^2))");
  CustomPolynomial cp( 10, -1.0, 1.0 );
  cp.setWeightFunction( user, 1e-6 );

  auto& r = cp.getRoots();

  auto& a = cp.getB();

  EXPECT_EQ( a.size(), 10 );

  double sum = 0;
  auto& w = cp.getWeights();
  for(size_t i = 0; i < a.size(); ++i)
  {
    std::cerr << i <<": "<<std::endl;
    std::cerr << "      " << a[i] << ' '  << std::endl;
    std::cerr << "      " << r[i]<< ' '  << std::endl;
    std::cerr << "      " << w[i]<< ' '  << std::endl;
    //EXPECT_NEAR( fabs(a[i]) - fabs(ap[i]), 0.0, 1e-13 );
    sum += w[i];
  }
  std::cerr << "Weight " << sum << ' ' << pi/2 << ' ' << pi/2 - cp.weightIntegral() << std::endl;
}
