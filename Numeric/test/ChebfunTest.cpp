#include "gtest/gtest.h"
#include "Numeric/Chebfun.h"
#include "Numeric/UserFunction1D.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include <iostream>
#include <cmath>
#include <boost/lexical_cast.hpp>

using namespace Numeric;

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
