#include "gtest/gtest.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

using namespace Numeric;

TEST(Numeric_JacobiPolynomial_Test, RootsTest)
{
  JacobiPolynomial P(1,1,11);
  auto& r = P.getRoots();

  FunctionDomain1DView d( r );
  FunctionValues v( d );
  P.function( d, v );

  EXPECT_EQ( r.size(), 11 );
  for(size_t i = 0; i < r.size(); ++i)
  {
    //if ( out )
    {
      std::cerr << i << ' ' << r[i] << ' ' << v.getCalculated(i) << std::endl;
    }
    //EXPECT_NE( r[i], 0 );
    //EXPECT_NEAR( v.getCalculated(i), 0, 1e-9 );
  }
}

TEST(Numeric_JacobiPolynomial_Test, Roots0505Test)
{
  JacobiPolynomial P(0.5,0.5,9);
  auto& r = P.getRoots();
  auto& a = P.getA();
  auto& b = P.getB();
  auto& c = P.getC();

  FunctionDomain1DView d( r );
  FunctionValues v( d );
  P.function( d, v );

  //EXPECT_EQ( r.size(), 10 );
  for(size_t i = 0; i < r.size(); ++i)
  {
    //if ( out )
    {
      std::cerr << i <<":" << std::endl;
      std::cerr << "    " << r[i] << ' ' << v.getCalculated(i) << std::endl;
      std::cerr << "    " << a[i] << ' ' << b[i] << ' ' << c[i] << std::endl;
    }
    //EXPECT_NE( r[i], 0 );
    //EXPECT_NEAR( v.getCalculated(i), 0, 1e-9 );
  }
}

