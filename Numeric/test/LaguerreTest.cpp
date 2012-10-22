#include "gtest/gtest.h"
#include "Numeric/Laguerre.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/Chebfun.h"

using namespace Numeric;

void LaguerreRootTest(Laguerre& L, Laguerre& L1, bool out = false)
{
  std::vector<double> r;
  L.myroots(r);
  auto& r0 = L1.getRoots();

  FunctionDomain1DView d( r );
  FunctionValues v( d );
  L.function( d, v );

  FunctionDomain1DView d0( r0 );
  FunctionValues v0( d0 );
  L1.function( d0, v0 );

  EXPECT_EQ( r.size(), 10 );
  EXPECT_EQ( r0.size(), 10 );
  for(size_t i = 0; i < r0.size(); ++i)
  {
    if ( out )
    {
      std::cerr << i << ' ' << r[i] << ' ' << r0[i] << ' ' << r[i] - r0[i];
      std::cerr << ' ' << v.getCalculated(i)  << ' ' << v0.getCalculated(i) << std::endl;
    }
    EXPECT_NE( r[i], 0 );
    EXPECT_NEAR( r[i], r0[i], 1e-13 );
    EXPECT_NEAR( v.getCalculated(i), 0, 1e-9 );
    EXPECT_NEAR( v0.getCalculated(i), 0, 1e-8 );
  }
}

TEST(Numeric_Laguerre_Test, RootsAlpha0Test)
{
  Laguerre L10(0,10);
  Laguerre L10_1(0,10);

  LaguerreRootTest(L10,L10_1);
}

TEST(Numeric_Laguerre_Test, RootsAlphaNonZeroTest)
{
  const int N = 10;
  const double alpha = 2.5;
  Laguerre L(alpha,N);
  Laguerre L1(alpha,N);

  LaguerreRootTest(L,L1);
}

TEST(Numeric_Laguerre_Test, WeightsAlpha0Test)
{
  Laguerre L(0,10);

  auto& r = L.getRoots();
  auto& w = L.getWeights();

  EXPECT_EQ( w.size(), 10 );

  double d0 = 0;
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  for(size_t i = 0; i < w.size(); ++i)
  {
    //std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
    double x = r[i];
    d0 += w[i];
    d1 += x * w[i];
    x *= r[i];
    d2 += x * w[i];
    x *= r[i];
    d3 += x * w[i];
  }
  
  EXPECT_NEAR( d0, 1.0, 1e-14); // integr( weight )
  EXPECT_NEAR( d1, 1.0, 1e-14); // integr( x*weight )
  EXPECT_NEAR( d2, 2.0, 1e-14); // integr( x^2*weight )
  EXPECT_NEAR( d3, 6.0, 1e-13); // integr( x^3*weight )

  //std::cerr << "d=" << d0 << ' ' << d1 << ' ' << d2 << ' ' << d3 << std::endl;
}

TEST(Numeric_Laguerre_Test, WeightsAlphaNonZeroTest)
{
  const double alpha = 3;
  Laguerre L(alpha,10);

  auto& r = L.getRoots();
  auto& w = L.getWeights();

  EXPECT_EQ( w.size(), 10 );

  double d0 = 0;
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  for(size_t i = 0; i < w.size(); ++i)
  {
    //std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
    double x = r[i];
    d0 += w[i];
    d1 += x * w[i];
    x *= r[i];
    d2 += x * w[i];
    x *= r[i];
    d3 += x * w[i];
  }
  
  EXPECT_NEAR( d0, 6.0, 1e-12);   // (alpha + 0)!
  EXPECT_NEAR( d1, 24.0, 1e-12);  // (alpha + 1)!
  EXPECT_NEAR( d2, 120.0, 1e-12); // (alpha + 2)!
  EXPECT_NEAR( d3, 720.0, 1e-12); // (alpha + 3)!

  //std::cerr << "d=" << d0 << ' ' << d1 << ' ' << d2 << ' ' << d3 << std::endl;
}

TEST(Numeric_Laguerre_Test, PartialTest)
{
  Laguerre L(0,10);

  auto& r = L.getRoots();
  auto& w = L.getWeights();
  //for(size_t i = 0; i < w.size(); ++i)
  //{
  //  std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
  //}

  std::set<size_t> ri;
  ri.insert(0);
  ri.insert(1);
  ri.insert(2);
  std::vector<double> rp; // partial roots
  std::vector<double> wp; // partial weights

  //system("pause");
  L.partialQuadrature( ri, rp, wp );
  EXPECT_EQ( rp.size(), 3 );
  EXPECT_EQ( wp.size(), 3 );
  double d0 = 0;
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  for(size_t i = 0; i < wp.size(); ++i)
  {
    std::cerr << i << ' ' << rp[i] << ' ' << wp[i] << std::endl;
    double x = rp[i];
    d0 += wp[i];
    d1 += x * wp[i];
    x *= rp[i];
    d2 += x * wp[i];
    x *= rp[i];
    d3 += x * wp[i];
  }
  std::cerr << "d=" << d0 << ' ' << d1 << ' ' << d2 << ' ' << d3 << std::endl;
  EXPECT_NEAR( d0, 1.0, 1e-14); // integr( weight )
  EXPECT_NEAR( d1, 1.0, 1e-14); // integr( x*weight )
  EXPECT_NEAR( d2, 2.0, 1e-14); // integr( x^2*weight )
  EXPECT_LT( d3, 6.0); // integr( x^3*weight )
}


TEST(Numeric_Laguerre_Test, BarycentricTest)
{
  Laguerre L(0,10);

  auto& r = L.getRoots();
  auto& w = L.getWeights();

  std::set<size_t> ri;
  ri.insert(0);
  ri.insert(1);
  ri.insert(2);
  std::vector<double> baryw; // barycentric weights
  
  L.calcBarycentricWeights(ri, baryw );
  std::vector<double> fx(3),fx2(3),fx3(3);
  fx[0] = r[0];
  fx[1] = r[1];
  fx[2] = r[2];

  fx2[0] = r[0]*r[0];
  fx2[1] = r[1]*r[1];
  fx2[2] = r[2]*r[2];

  fx3[0] = 2*r[0]*r[0] + r[0] + 1.0;
  fx3[1] = 2*r[1]*r[1] + r[1] + 1.0;
  fx3[2] = 2*r[2]*r[2] + r[2] + 1.0;

  for(size_t i = 3; i < r.size(); ++i)
  {
    double x = 0;
    double x2 = 0;
    double x3 = 0;
    double s = 0;
    for(size_t j = 0; j < baryw.size(); ++j)
    {
      const double t = baryw[j] / (r[i] - r[j]);
      x += fx[j] * t;
      x2 += fx2[j] * t;
      x3 += fx3[j] * t;
      s += t;
    }
    x /= s;
    x2 /= s;
    x3 /= s;
    //std::cerr << i << ':' << std::endl;
    //std::cerr << "     " << x << ' ' << r[i] << std::endl;
    //std::cerr << "     " << x2 << ' ' << r[i]*r[i] << std::endl;
    //std::cerr << "     " << x3 << ' ' << 2*r[i]*r[i] + r[i] + 1.0 << std::endl;
    EXPECT_NEAR( x, r[i], 1e-9 );
    EXPECT_NEAR( x2, r[i]*r[i], 1e-9 );
    EXPECT_NEAR( x3, 2*r[i]*r[i] + r[i] + 1.0, 1e-9 );
  }
}

TEST(Numeric_Laguerre_Test, SubPolynomialTest)
{
  Laguerre L10(0,10);
  Laguerre L3(0,3);
  auto sL = L10.subPolynomial( 3 );

  auto& r3 = L3.getRoots();
  auto& w3 = L3.getWeights();

  auto& sr = sL->getRoots();
  auto& sw = sL->getWeights();

  EXPECT_EQ( L3.n(), sL->n() );

  for(size_t i = 0; i < r3.size(); ++i)
  {
    //std::cerr << "i:" << std::endl;
    //std::cerr << r3[i] << ' ' << sr[i] << std::endl;
    //std::cerr << w3[i] << ' ' << sw[i] << std::endl;
    EXPECT_NEAR( r3[i], sr[i], 1e-15 );
    EXPECT_NEAR( w3[i], sw[i], 1e-15 );
  }
}

TEST(Numeric_Laguerre_Test, Partial2Test)
{
  Laguerre L(0,10);
  auto& r = L.getRoots();
  auto& w = L.getWeights();
  //for(size_t i = 0; i < w.size(); ++i)
  //{
  //  std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
  //}
  Laguerre L3(0,3);
  auto& r3 = L3.getRoots();
  auto& w3 = L3.getWeights();

  std::set<size_t> ri;
  ri.insert(0);
  ri.insert(1);
  ri.insert(2);
  std::vector<double> rp; // partial roots
  std::vector<double> wp; // partial weights

  //system("pause");
  L.partialQuadrature2( ri, rp, wp );
  EXPECT_EQ( rp.size(), 3 );
  EXPECT_EQ( wp.size(), 3 );
  double d0 = 0;
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  double d3_L3 = 0;
  for(size_t i = 0; i < wp.size(); ++i)
  {
    std::cerr << i << ' ' << rp[i] << ' ' << wp[i] << std::endl;
    double x = rp[i];
    d0 += wp[i];
    d1 += x * wp[i];
    x *= rp[i];
    d2 += x * wp[i];
    x *= rp[i];
    d3 += x * wp[i];
    d3_L3 += r3[i] * r3[i] * r3[i] * w3[i];
  }
  std::cerr << "d=" << d0 << ' ' << d1 << ' ' << d2 << ' ' << d3 << ' ' << d3_L3 << std::endl;
  EXPECT_NEAR( d0, 1.0, 1e-14); // integr( weight )
  EXPECT_NEAR( d1, 1.0, 1e-14); // integr( x*weight )
  EXPECT_NEAR( d2, 2.0, 1e-14); // integr( x^2*weight )
  EXPECT_LT( d3, 6.0); // integr( x^3*weight )
}

TEST(Numeric_Laguerre_Test, PolynomialDerivativeTest)
{
  Laguerre L(0,10);
  auto& r = L.getRoots();
  auto der = L.derivative();

  FunctionDomain1DVector domain(r.front(), r.back(), 10);
  FunctionValues der_values( domain );
  der->function( domain, der_values );

  chebfun cheb(10,r.front(), r.back());
  cheb.fit( L );
  chebfun cheb_der(10,r.front(), r.back());
  cheb_der.fromDerivative( cheb );

  for(size_t i = 0; i < domain.size(); ++i)
  {
    //std::cerr << domain[i] << ' ' << der_values.getCalculated(i) - cheb_der(domain[i]) << std::endl;
    EXPECT_NEAR( der_values.getCalculated(i),  cheb_der(domain[i]), 1e-10 );
  }
}
