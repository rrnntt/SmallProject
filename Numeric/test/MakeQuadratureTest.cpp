#include "gtest/gtest.h"
#include "Numeric/MakeQuadrature.h"
#include "Numeric/Quadrature.h"
#include "Numeric/Hermite.h"
#include "Numeric/Constants.h"

#include <gsl/gsl_sf_gamma.h>

using namespace Numeric;

double HermiteIntegral(size_t n)
{
  return sqrt( pi ) * exp( gsl_sf_lngamma(n+1) + n *log(2.0) );
}

TEST(Numeric_MakeQuadrature_Test, UnnormalizedTest)
{
  const size_t n = 10;
  
  Hermite H(n);
  auto& rh = H.getRoots();
  auto& wh = H.getWeights();
  
  MakeQuadrature alg;
  alg.setProperty("Type","Hermite");
  alg.setProperty("N", int(n));
  alg.setProperty("Normalize", false);
  alg.execute();
  boost::shared_ptr<Quadrature> quad = alg.getClass("Quadrature");
  EXPECT_TRUE( quad );
  EXPECT_EQ( quad->rowCount(), n );
  auto& r = quad->getDoubleData("r");
  auto& w = quad->getDoubleData("w");
  auto& f0 = quad->getDoubleData("f0");
  auto& f1 = quad->getDoubleData("f1");
  auto& f2 = quad->getDoubleData("f2");
  auto& f3 = quad->getDoubleData("f3");

  double sum = 0.0;
  double sum_f0 = 0.0;
  double sum_f1 = 0.0;
  double sum_f2 = 0.0;
  double sum_f3 = 0.0;
  for(size_t i = 0; i < n; ++i)
  {
    EXPECT_EQ( r[i], rh[i] );
    EXPECT_EQ( w[i], wh[i] );
    //std::cerr << i << ' ' << r[i] << ' ' << w[i] << ' ' << wh[i] << std::endl;
    sum += w[i];
    sum_f0 += f0[i] * f0[i];
    sum_f1 += f1[i] * f1[i];
    sum_f2 += f2[i] * f2[i];
    sum_f3 += f3[i] * f3[i];
  }
  EXPECT_NEAR( sum, H.weightIntegral(), 1e-10 );
  EXPECT_NEAR( sum_f0, HermiteIntegral(0), 1e-10 );
  EXPECT_NEAR( sum_f1, HermiteIntegral(1), 1e-10 );
  EXPECT_NEAR( sum_f2, HermiteIntegral(2), 1e-10 );
  EXPECT_NEAR( sum_f3, HermiteIntegral(3), 1e-10 );
  std::cerr << sum << ' ' << H.weightIntegral() << std::endl;
  std::cerr << sum_f0 << ' ' << HermiteIntegral(0) << std::endl;
  std::cerr << sum_f1 << ' ' << HermiteIntegral(1) << std::endl;
  std::cerr << sum_f2 << ' ' << HermiteIntegral(2) << std::endl;
  std::cerr << sum_f3 << ' ' << HermiteIntegral(3) << std::endl;
}

TEST(Numeric_MakeQuadrature_Test, NormalizedTest)
{
  const size_t n = 10;
  
  Hermite H(n);
  auto& rh = H.getRoots();
  auto& wh = H.getWeights();
  
  MakeQuadrature alg;
  alg.setProperty("Type","Hermite");
  alg.setProperty("N", int(n));
  alg.setProperty("Normalize", true);
  alg.execute();
  boost::shared_ptr<Quadrature> quad = alg.getClass("Quadrature");
  EXPECT_TRUE( quad );
  EXPECT_EQ( quad->rowCount(), n );
  auto& r = quad->getDoubleData("r");
  auto& w = quad->getDoubleData("w");
  auto& f0 = quad->getDoubleData("f0");
  auto& f1 = quad->getDoubleData("f1");
  auto& f2 = quad->getDoubleData("f2");
  auto& f3 = quad->getDoubleData("f3");
  auto& f9 = quad->getDoubleData("f9");

  double sum = 0.0;
  double sum_f0 = 0.0;
  double sum_f1 = 0.0;
  double sum_f2 = 0.0;
  double sum_f3 = 0.0;
  double sum_f9 = 0.0;
  for(size_t i = 0; i < n; ++i)
  {
    EXPECT_EQ( r[i], rh[i] );
    EXPECT_EQ( w[i], wh[i] );
    //std::cerr << i << ' ' << r[i] << ' ' << w[i] << ' ' << wh[i] << std::endl;
    sum += w[i];
    sum_f0 += f0[i] * f0[i];
    sum_f1 += f1[i] * f1[i];
    sum_f2 += f2[i] * f2[i];
    sum_f3 += f3[i] * f3[i];
    sum_f9 += f9[i] * f9[i];
  }
  EXPECT_NEAR( sum_f0, 1, 1e-10 );
  EXPECT_NEAR( sum_f1, 1, 1e-10 );
  EXPECT_NEAR( sum_f2, 1, 1e-10 );
  EXPECT_NEAR( sum_f3, 1, 1e-10 );
  EXPECT_NEAR( sum_f9, 1, 1e-10 );
  std::cerr << sum_f0 << std::endl;
  std::cerr << sum_f1 << std::endl;
  std::cerr << sum_f2 << std::endl;
  std::cerr << sum_f3 << std::endl;
  std::cerr << sum_f9 << std::endl;
}

TEST(Numeric_MakeQuadrature_Test, BCoeffsTest)
{
  const size_t n = 50;
  
  Hermite H(n);
  auto& r = H.getRoots();
  auto& w = H.getWeights();
  // just testing here the way of calculating base polynomials norms
  // using the b-coefficiants.

  auto& b = H.getB();
  double test = HermiteIntegral( 0 );
  for(size_t i = 0; i < n; ++i)
  {
    if ( i > 0 ) test *= b[i];
    //std::cerr << i << ' ' << b[i] << ' ' << HermiteIntegral( i ) << ' ' << test << std::endl;
    EXPECT_NEAR(HermiteIntegral( i ) / test, 1.0, 1e-13);
  }
}