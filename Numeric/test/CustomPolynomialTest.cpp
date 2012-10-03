#include "gtest/gtest.h"
#include "Numeric/CustomPolynomial.h"
#include "Numeric/Laguerre.h"
#include "Numeric/LaguerreWeight.h"
#include "Numeric/JacobiPolynomial.h"

using namespace Numeric;

TEST(Numeric_CustomPolynomial_Test, LaguerreTest)
{
  Laguerre lagP(0.0, 10);
  CustomPolynomial cp( 10, 0.0, 120.0 );
  cp.setWeightFunction( lagP.weightFunction() );

  auto& a = cp.getA();
  auto& al = lagP.getA();

  EXPECT_EQ( a.size(), 10 );

  for(size_t i = 0; i < a.size(); ++i)
  {
    //std::cerr << i << ' ' << a[i] << ' ' << al[i] << ' ' << fabs(a[i]) - fabs(al[i]) << std::endl;
    EXPECT_NEAR( fabs(a[i]) - fabs(al[i]), 0.0, 1e-13 );
  }
}

TEST(Numeric_CustomPolynomial_Test, JacobiTest)
{
  JacobiPolynomial P(1.0, 1.0, 10);
  CustomPolynomial cp( 10, -1.0, 1.0 );
  cp.setWeightFunction( P.weightFunction() );

  std::vector<double> r;
  P.roots( r );

  auto& a = cp.getB();
  auto& ap = P.getB();

  EXPECT_EQ( a.size(), 10 );

  for(size_t i = 0; i < a.size(); ++i)
  {
    std::cerr << i <<": "<<std::endl;
    std::cerr << "      " << a[i] << ' ' << ap[i] << ' ' << fabs(a[i]) - fabs(ap[i]) << std::endl;
    //std::cerr << "      " << r[i] << std::endl;
    //EXPECT_NEAR( fabs(a[i]) - fabs(ap[i]), 0.0, 1e-13 );
  }
}
