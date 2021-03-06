#define _USE_MATH_DEFINES
#include "gtest/gtest.h"
#include "Numeric/Chebfun.h"
#include "Numeric/UserFunction1D.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include <iostream>
#include <cmath>
#include <boost/lexical_cast.hpp>

using namespace Numeric;

//=========================================================================//
class TestCheb: public chebfun
{
public:
  TestCheb(int n = -1, const double& startX = -1,const double& endX = 1):
      chebfun(n,startX,endX){}
  TestCheb(const TestCheb& other):chebfun(other){}

  void setPs(const double* p, size_t n)
  {
    if (m_p.size() != n) throw std::runtime_error("Wrong size " + 
      boost::lexical_cast<std::string>(m_p.size()) + " " + boost::lexical_cast<std::string>(n)
      );
    for(size_t i = 0; i < n; ++i,++p)
    {
      m_p[i] = *p;
    }
    calcA();
  }
};

namespace
{
  double gauss( double x )
  {
    return exp(-x*x);
  }
  double xSquared( double x )
  {
    return (1.0 + x)*(1.0 + x);
  }
  double unit( double x )
  {
    return 1.0;
  }
  double xSquared2( double x )
  {
    return x * x;
  }
};

//=========================================================================//

TEST(ChebfunTest, XPointsTest)
{
  chebfun cheb(5,-1.5,1.5);
  auto& x = cheb.xpoints();
  EXPECT_EQ( x[0], -1.5 );
  EXPECT_EQ( x[5], 1.5 );
  EXPECT_EQ( x.size(), 6 );
  EXPECT_EQ( cheb.startX(), -1.5 );
  EXPECT_EQ( cheb.endX(), 1.5 );
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, FitTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x)");
  FunctionDomain1DVector x(-1.5,1.5,30);
  FunctionValues y(x);
  user.function(x,y);
  
  chebfun cheb(5,-1.5,1.5);
  cheb.fit(user);

  EXPECT_EQ(cheb.n(),5);
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR(y.getCalculated(i),cheb(x[i]),1e-4);
  }

  auto& xp = cheb.xpoints();
  auto& yp = cheb.ypoints();
  for(size_t i = 0; i < xp.size(); ++i)
  {
    const double xx = xp[i];
    EXPECT_EQ( yp[i], cheb( xx ) );
    //std::cerr << xx << ' ' << yp[i] << ' ' << cheb.valueT( xx ) << std::endl;
  }

  chebfun cheb1(15,-1.5,1.5);
  cheb1.fit(user);
  auto& a = cheb1.coeffs();
  double a0 = 10;
  for(size_t i = 0; i < a.size(); ++i)
  {
    if ( i % 2 == 0 )
    {
      EXPECT_NEAR(0.0, a[i], 1e-14);
    }
    else
    {
      EXPECT_LT( fabs(a[i]/a0), 0.12 );
      a0 = a[i];
    }
    //std::cerr << i << ' ' << a[i] << std::endl;
  }
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR(y.getCalculated(i),cheb1(x[i]),1e-14);
    EXPECT_NEAR(cheb1.valueT( x[i] ),cheb1(x[i]),1e-13);
    //std::cerr << y.getCalculated(i) << ' ' << cheb1(x[i]) << ' ' <<  << std::endl;
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, CopyTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x)");
  FunctionDomain1DVector x(-1.5,1.5,30);
  FunctionValues y(x);
  user.function(x,y);
  
  TestCheb cheb(10,-1.5,1.5);
  cheb.fit(user);

  TestCheb cheb1 = cheb;

  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_EQ(cheb1(x[i]),cheb(x[i]));
  }

  EXPECT_TRUE( cheb1.haveSameBase(cheb) );
  EXPECT_EQ( cheb.n(),cheb1.n());
  
  TestCheb cheb2;

  cheb2 = cheb1;
  EXPECT_TRUE( cheb2.haveSameBase(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  cheb2 += cheb1;
  EXPECT_TRUE( cheb2.haveSameBase(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),2*cheb(x[i]));
  }

  cheb2 -= cheb1;
  EXPECT_TRUE( cheb2.haveSameBase(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  cheb2 *= cheb1;
  EXPECT_TRUE( cheb2.haveSameBase(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb2(x[i]) - cheb(x[i]) * cheb(x[i]) << std::endl;
    EXPECT_NEAR(cheb2(x[i]),cheb(x[i]) * cheb(x[i]),1e-6);
  }

  cheb2 /= cheb1;
  EXPECT_TRUE( cheb2.haveSameBase(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  TestCheb cheb3(5,-1.5,1.5);
  cheb3.fit(user);
  EXPECT_FALSE( cheb3.haveSameBase(cheb) );

  cheb3 += cheb1;
  EXPECT_FALSE( cheb3.haveSameBase(cheb) );
  EXPECT_NE(cheb3.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb3(x[i]) - 2*cheb(x[i]) << std::endl;
    EXPECT_NEAR(cheb3(x[i]),2*cheb(x[i]),1e-3);
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, UniformFitTest)
{
  const double PI = 2*acos(0.0);

  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x-0.2)");
  FunctionDomain1DVector x(-1.5,1.5,31);
  FunctionValues y(x);
  user.function(x,y);

  TestCheb cheb(30,-1,1);
  cheb.setPs(y.getPointerToCalculated(0),x.size());
  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = x[i];
    double xx1 = -cos(PI*(xx + 1.5)/(3.0));
    //std::cerr << xx << ' ' << xx1 << ' ' << cheb(xx1) - y.getCalculated(i) << std::endl;
    EXPECT_NEAR(cheb(xx1),y.getCalculated(i),1e-15);
  }

  std::vector<double> yvec;
  yvec.assign(y.getPointerToCalculated(0),y.getPointerToCalculated(0) + y.size());
  EXPECT_EQ(yvec.size(), y.size());

  chebfun ucheb;
  ucheb.uniformFit(-1.5,1.5,yvec);
  EXPECT_GT(ucheb.n(), yvec.size()-1);
  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = x[i];
    //std::cerr << xx << ' ' << ucheb(xx) - y.getCalculated(i) << std::endl;
    EXPECT_NEAR(ucheb(xx),y.getCalculated(i),2e-15);
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, IntegrateTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x)");
  
  TestCheb cheb(15,0,M_PI);
  cheb.fit(user);
  EXPECT_NEAR( cheb.integr(), 2.0, 1e-15 );
  //std::cerr << cheb.integr() << std::endl;
  
  TestCheb cheb1(15,0,3*M_PI/4);
  cheb1.fit(user);
  EXPECT_NEAR( cheb1.integr(), cos(0.0) - cos(3*M_PI/4), 1e-15 );
  //std::cerr << cheb1.integr() << ' ' << ( cos(0.0) - cos(3*M_PI/4)) << ' ' << cheb1.integrate() << std::endl;
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, IntegrateGaussTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","exp(-1*x^2)");
  
  chebfun cheb(100,-10.0,10.0);
  cheb.fit(user);
  auto& a = cheb.coeffs();
  auto &p = cheb.ypoints();
  size_t n = cheb.n();

  for(size_t i = 0; i < a.size(); ++i)
  {
    double b = 0.0;
    for(size_t j = 0; j < a.size(); ++j)
    {
      double t = p[j];
      if ( j == 0 || j == n ) t /= 2;
      b += t * cos(M_PI*double(i*j)/n);
    }
    b /= n;
    if ( i != 0 && i != n ) b *= 2;
    if ( i % 2 == 0 )
    std::cerr << i << ' ' << a[i] << ' ' << b << ' ' << a[i] - b << std::endl;
  }

  EXPECT_NEAR( cheb.integr(), sqrt(M_PI), 1e-15 );
  EXPECT_NEAR( cheb.integrate(), sqrt(M_PI), 1e-15 );
  std::cerr << cheb.integr() - sqrt(M_PI) << std::endl;
  std::cerr << cheb.integr() << ' ' << sqrt(M_PI) << std::endl;
  std::cerr << cheb.integrate() - sqrt(M_PI) << std::endl;
}
  
//-----------------------------------------------------------------------------
TEST(ChebfunTest, CalcATest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x-0.3)");
  
  TestCheb cheb(15,0,3*M_PI/4);
  cheb.fit(user);

  auto a = cheb.coeffs();
  // copy of the y-points
  auto p0 = cheb.ypoints();

  cheb.calcP();
  // current y-points
  auto& p = cheb.ypoints();

  for(size_t i = 0; i < p.size(); ++i)
  {
    EXPECT_NEAR( p[i] , p0[i], 1e-15 );
    a[i] *= 2;
  }
  cheb.setA( a );
  for(size_t i = 0; i < p.size(); ++i)
  {
    EXPECT_NEAR( p[i] , 2 * p0[i], 1e-15 );
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, CalcA1Test)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","4*x^3-3*x");
  
  chebfun cheb(5,-1,1);
  cheb.fit(user);

  auto a = cheb.coeffs();

  EXPECT_NEAR( a[0] , 0, 1e-15 );
  EXPECT_NEAR( a[1] , 0, 1e-15 );
  EXPECT_NEAR( a[2] , 0, 1e-15 );
  EXPECT_NEAR( a[3] , 1, 1e-15 );
  EXPECT_NEAR( a[4] , 0, 1e-15 );
  //for(size_t i = 0; i < a.size(); ++i)
  //{
  //  std::cerr << i << ' ' << a[i] << std::endl;
  //}
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, CalcA2Test)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","2*x^2-1");
  
  chebfun cheb(5,-1,1);
  cheb.fit(user);

  auto a = cheb.coeffs();

  EXPECT_NEAR( a[0] , 0, 1e-15 );
  EXPECT_NEAR( a[1] , 0, 1e-15 );
  EXPECT_NEAR( a[2] , 1, 1e-15 );
  EXPECT_NEAR( a[3] , 0, 1e-15 );
  EXPECT_NEAR( a[4] , 0, 1e-15 );
  //for(size_t i = 0; i < a.size(); ++i)
  //{
  //  std::cerr << i << ' ' << a[i] << std::endl;
  //}
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, CalcA3Test)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","x^2");
  
  chebfun cheb(2,-1,1);
  cheb.fit(user);

  auto a = cheb.coeffs();

  EXPECT_NEAR( a[0] , 0.5, 1e-15 );
  EXPECT_NEAR( a[1] , 0, 1e-15 );
  EXPECT_NEAR( a[2] , 0.5, 1e-15 );
  //for(size_t i = 0; i < a.size(); ++i)
  //{
  //  std::cerr << i << ' ' << a[i] << std::endl;
  //}
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, FromDerivativeTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x-0.2)");
  
  TestCheb cheb(15,0,M_PI/1.5);
  cheb.fit(user);

  chebfun der;
  der.fromDerivative( cheb );
  auto& x = cheb.xpoints();

  EXPECT_TRUE( der.haveSameBase( cheb ) );

  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( der.ypoints()[i], cos( x[i] - 0.2 ), 1e-13 );
    //std::cerr << x[i] << ' ' << der.ypoints()[i] << ' ' << cos( x[i] - 0.2 ) << std::endl;
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, FromDerivative2Test)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","sin(x-0.2)");
  
  TestCheb cheb(15,0,M_PI/1.5);
  cheb.fit(user);

  chebfun der;
  der.fromDerivative2( cheb );
  auto& x = cheb.xpoints();

  EXPECT_TRUE( der.haveSameBase( cheb ) );

  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( der.ypoints()[i], -sin( x[i] - 0.2 ), 1e-11 );
    //std::cerr << x[i] << ' ' << der.ypoints()[i] << ' ' << -sin( x[i] - 0.2 ) << std::endl;
  }
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, RootsTest)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","x-0.5");
  
  chebfun cheb( 4, -10.0, 2.0 );
  cheb.fit( user );
  std::vector<double> r;
  cheb.roots(r);

  for(size_t i = 0; i < r.size(); ++i)
  {
    std::cerr << "root " << i << " is " << r[i] << std::endl;
  }
  
  EXPECT_EQ( r.size(), 1 );
  EXPECT_NEAR( r[0], 0.5, 1e-10 );

}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, Roots1Test)
{
  UserFunction1D user;
  user.setAttributeValue("Formula","x-cos(4*x)");
  
  chebfun cheb( 19, -1.0, 1.0 );
  cheb.fit( user );
  std::vector<double> r;
  cheb.roots(r);

  //for(size_t i = 0; i < r.size(); ++i)
  //{
  //  std::cerr << "root " << i << " is " << r[i] << std::endl;
  //}

  EXPECT_EQ( r.size(), 3 );
  EXPECT_NEAR( r[0], -0.89882621679039, 1e-10 );
  EXPECT_NEAR( r[1], -0.53333306291483, 1e-10 );
  EXPECT_NEAR( r[2],  0.31308830850065, 1e-10 );

}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, BestFitTest)
{
  chebfun cheb(0, 0, pi);
  cheb.bestFit( (AFunction)sin );
  //std::cerr << "n=" << cheb.n() << std::endl;
  EXPECT_EQ( cheb.n(), 26 );

  chebfun cheb_gauss(0, 0, 4);
  cheb_gauss.bestFit( (AFunction)gauss );
  //std::cerr << "n=" << cheb_gauss.n() << std::endl;
  EXPECT_EQ( cheb_gauss.n(), 40 );
}

//-----------------------------------------------------------------------------
TEST(ChebfunTest, IntegrPolyTest)
{
  //system("pause");
  chebfun cheb(2);
  cheb.fit( xSquared );
  EXPECT_NEAR( cheb.integr(), 8./3, 1e-15);
  //std::cerr << "integr " << cheb.integr() << ' ' << cheb.integr2() << std::endl;
}

TEST(ChebfunTest, IntegrateWeightedTest)
{
  chebfun cheb(3);
  cheb.fit( xSquared2 );
  EXPECT_NEAR( cheb.integrateWeighted(), pi/8, 1e-15);
  //std::cerr << "integr " << pi/8 << ' ' << cheb.integrateWeighted() << std::endl;
  //auto& w = cheb.weights();
  //auto& x = cheb.xpoints();
  //auto& p = cheb.ypoints();
  //for(size_t i = 0; i < x.size(); ++i)
  //{
  //  std::cerr << i << ' ' << x[i] << ' ' << p[i] << ' ' << w[i] << std::endl;
  //}
}

// I want to test if chebfun's base functions are orthogonal
TEST(ChebfunTest, BaseOrthogonalityTest)
{
    chebfun cheb1(10);
    chebfun cheb2(cheb1);
    std::vector<double> p = cheb1.ypoints();
    p[4] = 1.0;
    cheb1.setP( p );
    std::cerr << "I1=" << cheb1.integr() << std::endl;
    p[4] = 0.0;
    p[0] = 1.0;
    cheb2.setP( p );
    std::cerr << "I2=" << cheb2.integr() << std::endl;
    cheb2 *= cheb1;
    std::cerr << "I12=" << cheb2.integr() << std::endl;
}

TEST(ChebfunTest, ConvolutionTest)
{
}
