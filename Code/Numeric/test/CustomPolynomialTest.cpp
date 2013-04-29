#include "gtest/gtest.h"
#include "Numeric/CustomPolynomial.h"
#include "Numeric/Laguerre.h"
#include "Numeric/LaguerreWeight.h"
#include "Numeric/JacobiPolynomial.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/Constants.h"

using namespace Numeric;

#define out_coeff(i, a, aP) std::cerr << i << std::setw(14) << a[i] << std::setw(14) << aP[i] << std::setw(14) << fabs(a[i]) / fabs(aP[i]) << std::endl;
#define get_coeffs() \
  auto& a = cp.getA();\
  auto& b = cp.getB();\
  auto& c = cp.getC();\
  auto& w = cp.getWeights();\
  auto& r = cp.getRoots();\
  \
  auto& aP = P.getA();\
  auto& bP = P.getB();\
  auto& cP = P.getC();\
  auto& wP = P.getWeights();\
  auto& rP = P.getRoots();\

TEST(Numeric_CustomPolynomial_Test, LaguerreTest)
{
    size_t n = 10;
  Laguerre P(1.0, n);
  CustomPolynomial cp( n, 0.0, 120.0 );
  cp.setWeightFunction( P.weightFunction() );

  get_coeffs()

  EXPECT_EQ( a.size(), n );

  double sum = 0;
  for(size_t i = 0; i < a.size(); ++i)
  {
      //out_coeff(i, r, rP );
      //out_coeff(i, w, wP );
//      out_coeff(i, a, aP );
//      out_coeff(i, b, bP );
//      out_coeff(i, c, cP );
      EXPECT_NEAR( fabs(r[i]) - fabs(rP[i]), 0.0, 1e-13 );
      EXPECT_NEAR( fabs(w[i]) - fabs(wP[i]), 0.0, 1e-13 );
    sum += w[i];
  }
  std::cerr << "Weight " << sum << ' ' << P.weightIntegral() << ' ' << cp.weightIntegral() << std::endl;
  EXPECT_NEAR( sum, P.weightIntegral(), 1e-9 );
  EXPECT_NEAR( sum, cp.weightIntegral(), 1e-9 );
}

class Xfun: public IFunction1D, public ParamFunction
{
public:
  /// Returns the function's name
  virtual std::string name()const {return "Xfun";}
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const
  {
    for(size_t i = 0; i < nData; ++i)
    {
      const double x = xValues[i];
      out[i] = x*exp(-x*x/2);
    }
  }
};


TEST(Numeric_CustomPolynomial_Test, JacobiTest)
{
  // it works only with even integer values for alpha and beta
  JacobiPolynomial P(4.0, 2.0, 10);
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
      out_coeff(i, r, rP );
      out_coeff(i, w, wP );
//      out_coeff(i, a, aP );
//      out_coeff(i, b, bP );
//      out_coeff(i, c, cP );
    //EXPECT_NEAR( fabs(a[i]) - fabs(ap[i]), 0.0, 1e-13 );
    sum += w[i];
  }
  std::cerr << "Weight " << sum << ' ' << P.weightIntegral() << ' ' << cp.weightIntegral() << std::endl;
}

TEST(Numeric_CustomPolynomial_Test, ChebyshevTest)
{
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
