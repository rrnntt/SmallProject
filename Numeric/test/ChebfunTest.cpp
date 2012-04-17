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
  bool hasSameX(TestCheb& fun)
  {
    return this->m_x == fun.m_x;
  }

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

  EXPECT_TRUE( cheb1.hasSameX(cheb) );
  EXPECT_EQ( cheb.n(),cheb1.n());
  
  TestCheb cheb2;

  cheb2 = cheb1;
  EXPECT_TRUE( cheb2.hasSameX(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  cheb2 += cheb1;
  EXPECT_TRUE( cheb2.hasSameX(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),2*cheb(x[i]));
  }

  cheb2 -= cheb1;
  EXPECT_TRUE( cheb2.hasSameX(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  cheb2 *= cheb1;
  EXPECT_TRUE( cheb2.hasSameX(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb2(x[i]) - cheb(x[i]) * cheb(x[i]) << std::endl;
    EXPECT_NEAR(cheb2(x[i]),cheb(x[i]) * cheb(x[i]),1e-6);
  }

  cheb2 /= cheb1;
  EXPECT_TRUE( cheb2.hasSameX(cheb) );
  EXPECT_EQ(cheb2.n(),cheb1.n());
  for(size_t i = 0; i < x.size(); ++i)
  {
    //std::cerr << x[i] << ' ' << cheb1(x[i]) - cheb2(x[i]) << std::endl;
    EXPECT_EQ(cheb2(x[i]),cheb(x[i]));
  }

  TestCheb cheb3(5,-1.5,1.5);
  cheb3.fit(user);
  EXPECT_FALSE( cheb3.hasSameX(cheb) );

  cheb3 += cheb1;
  EXPECT_FALSE( cheb3.hasSameX(cheb) );
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
    double xx1 = cos(PI*(xx + 1.5)/(3.0));
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
