#include "gtest/gtest.h"
#include "Numeric/Laguerre.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

using namespace Numeric;

void LaguerreRootTest(Laguerre& L, Laguerre& L1, bool out = false)
{
  std::vector<double> r,r0;
  L.myroots(r);
  L1.roots(r0);

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

TEST(Numeric_Laguerre_Test, ScaledRootsAlpha0Test)
{
  Laguerre L(0,10);
  Laguerre L0(0,10);

  L.setParameter("Scaling", 2.2);
  L0.setParameter("Scaling", 2.2);

  LaguerreRootTest(L,L0);
}

TEST(Numeric_Laguerre_Test, ScaledRootsAlphaNonZeroTest)
{
  Laguerre L(2.5,10);
  Laguerre L0(2.5,10);

  L.setParameter("Scaling", 2.2);
  L0.setParameter("Scaling", 2.2);

  LaguerreRootTest(L,L0);
}

