#include "gtest/gtest.h"
#include "Numeric/LagrangeInterpolation.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/Laguerre.h"

using namespace Numeric;

TEST(Numeric_LagrangeInterpolation_Test, LinearTest)
{
  //system("pause");
  std::vector<double> r;
  r.push_back( 0.0 );
  r.push_back( 1.0 );
  r.push_back( 2.0 );
  LagrangeInterpolation L( r );

  for(size_t i = 0; i < r.size(); ++i)
  {
    const double x = r[i];
    L.setValue( i, 2*x );
  }

  FunctionDomain1DVector domain(-1,3,10);
  FunctionValues values(domain);
  L.function(domain,values);

  for(size_t i = 0; i < domain.size(); ++i)
  {
    const double x = domain[i];
    const double y = values.getCalculated(i);
    EXPECT_NEAR( y, 2*x , 1e-15 );
    //std::cerr << i << ' ' << x << ' ' << y << ' ' << 2*x << std::endl;
  }
}

TEST(Numeric_LagrangeInterpolation_Test, LaguerreQuadratureTest)
{
  Laguerre laguerre11(0,11);
  auto r11 = laguerre11.getRoots();
  auto w11 = laguerre11.getWeights();
  
  FunctionDomain1DView roots11( r11 );

  // calc values of laguerre weight fnction at integration points
  FunctionValues weightFun( roots11 );
  laguerre11.weightFunction()->function( roots11, weightFun );

  Laguerre laguerre10(0,10);
  auto r10 = laguerre10.getRoots();
  auto w10 = laguerre10.getWeights();
  FunctionDomain1DView roots10( r10 );

  // check the weights calculated by integrating basis function of lagrange interpolation
  LagrangeInterpolation L10( r10 );

  for(size_t k = 0; k < r10.size(); ++k)
  {
    if ( k > 0 )
    {
      L10.setValue( k - 1, 0 );
    }
    L10.setValue( k, 1 );
    FunctionValues L10Values11( roots11 );
    L10.function( roots11, L10Values11 );
    double lw10 = 0;
    for(size_t i = 0; i < r11.size(); ++i)
    {
      lw10 += L10Values11.getCalculated(i) * w11[i];
    }
    //std::cerr << "wgt " << lw10 << ' ' << w10[k] << ' ' << lw10 - w10[k] << std::endl;
    EXPECT_NEAR( lw10, w10[k], 1e-15 );
  }

  std::vector<double> r3;
  r3.assign( r10.begin(), r10.begin() + 3 );
  std::vector<double> w3(3);
  LagrangeInterpolation L3( r3 );

  for(size_t k = 0; k < r3.size(); ++k)
  {
    if ( k > 0 )
    {
      L3.setValue( k - 1, 0 );
    }
    L3.setValue( k, 1 );
    FunctionValues L3Values10( roots10 );
    L3.function( roots10, L3Values10 );
    double lw3 = 0;
    for(size_t i = 0; i < r10.size(); ++i)
    {
      lw3 += L3Values10.getCalculated(i) * w10[i];
    }
    w3[k] = lw3;
    std::cerr << "wgt " << lw3 << ' ' << w10[k] << ' ' << lw3 - w10[k] << std::endl;
    //EXPECT_NEAR( lw3, w10[k], 1e-15 );
  }

  double d0 = 0;
  double d1 = 0;
  double d2 = 0;
  double d3 = 0;
  for(size_t i = 0; i < w3.size(); ++i)
  {
    //std::cerr << i << ' ' << r[i] << ' ' << w[i] << std::endl;
    double x = r3[i];
    d0 += w3[i];
    d1 += x * w3[i];
    x *= r3[i];
    d2 += x * w3[i];
    x *= r3[i];
    d3 += x * w3[i];
  }
  
  //EXPECT_NEAR( d0, 1.0, 1e-14); // integr( weight )
  //EXPECT_NEAR( d1, 1.0, 1e-14); // integr( x*weight )
  //EXPECT_NEAR( d2, 2.0, 1e-14); // integr( x^2*weight )
  //EXPECT_NEAR( d3, 6.0, 1e-13); // integr( x^3*weight )

  std::cerr << "d=" << d0 << ' ' << d1 << ' ' << d2 << ' ' << d3 << std::endl;
}
