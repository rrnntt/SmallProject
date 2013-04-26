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
    size_t n = 5;
  Laguerre P(0.0, n);
  CustomPolynomial cp( n, 0.0, 120.0 );
  cp.setWeightFunction( P.weightFunction() );

  auto& a = cp.getA();
  auto& b = cp.getB();
  auto& c = cp.getC();
  auto& w = cp.getWeights();
  auto& r = cp.getRoots();

  auto& aP = P.getA();
  auto& bP = P.getB();
  auto& cP = P.getC();
  auto& wP = P.getWeights();
  auto& rP = P.getRoots();

  EXPECT_EQ( a.size(), n );

  double sum = 0;
  for(size_t i = 0; i < a.size(); ++i)
  {
//      std::cerr << i << ' ' << r[i] << ' ' << rP[i] << ' ' << fabs(r[i]) - fabs(rP[i]) << std::endl;
      std::cerr << i << ' ' << w[i] << ' ' << wP[i] << ' ' << fabs(w[i]) - fabs(wP[i]) << std::endl;
//      std::cerr << i << ' ' << a[i] << ' ' << aP[i] << ' ' << fabs(a[i]) - fabs(aP[i]) << std::endl;
//      std::cerr << i << ' ' << b[i] << ' ' << bP[i] << ' ' << fabs(b[i]) - fabs(bP[i]) << std::endl;
//      std::cerr << i << ' ' << c[i] << ' ' << cP[i] << ' ' << fabs(c[i]) - fabs(cP[i]) << std::endl;
      EXPECT_NEAR( fabs(r[i]) - fabs(rP[i]), 0.0, 1e-13 );
      //EXPECT_NEAR( fabs(w[i]) - fabs(wP[i]), 0.0, 1e-6 );
    sum += w[i];
  }
  //std::cerr << "Weight " << sum << ' ' << P.weightIntegral() << ' ' << cp.weightIntegral() << std::endl;
  //EXPECT_NEAR( sum, P.weightIntegral(), 1e-9 );
  //EXPECT_NEAR( sum, cp.weightIntegral(), 1e-9 );
}

TEST(Numeric_CustomPolynomial_Test, JacobiTest)
{
  JacobiPolynomial P(1.0, 1.0, 10);
  CustomPolynomial cp( 10, -1.0, 1.0 );
  cp.setWeightFunction( P.weightFunction() );

  auto& a = cp.getA();
  auto& b = cp.getB();
  auto& c = cp.getC();
  auto& w = cp.getWeights();
  auto& r = cp.getRoots();

  auto& aP = P.getA();
  auto& bP = P.getB();
  auto& cP = P.getC();
  auto& wP = P.getWeights();
  auto& rP = P.getRoots();

  EXPECT_EQ( a.size(), 10 );

  double sum = 0;
  for(size_t i = 0; i < a.size(); ++i)
  {
    std::cerr << i <<": "<<std::endl;
    std::cerr << "      " << a[i] << ' ' << aP[i] << ' ' << fabs(a[i]) - fabs(aP[i]) << std::endl;
    std::cerr << "      " << b[i] << ' ' << bP[i] << ' ' << fabs(b[i]) - fabs(bP[i]) << std::endl;
    std::cerr << "      " << c[i] << ' ' << cP[i] << ' ' << fabs(c[i]) - fabs(cP[i]) << std::endl;
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
