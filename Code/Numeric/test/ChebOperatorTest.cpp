#include "gtest/gtest.h"
#include "Numeric/ChebOperator.h"
#include "Numeric/FunctionFactory.h"

using namespace Numeric;

TEST(Numeric_ChebOperator_Test, DiffTest)
{
  ChebDiff diff;
  chebfun c(15,-1.4,1.6);
  c.fit(sin);

  chebfun d1;
  d1.fromDerivative( c );

  chebfun d( c );
  diff.apply( c, d );

  auto& x = d.xpoints();
  auto& y = d.ypoints();
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( y[i], cos(x[i]), 1e-6 );
    EXPECT_NEAR( d1(x[i]), cos(x[i]), 1e-6 );
    //std::cerr << x[i] << ' ' << y[i] << ' ' << d1(x[i]) << ' ' <<  << std::endl;
  }
}

TEST(Numeric_ChebOperator_Test, Diff2Test)
{
  ChebDiff2 diff2;
  chebfun c(15,-1.4,1.6);
  c.fit(sin);

  chebfun d1;
  d1.fromDerivative( c );
  chebfun d2;
  d2.fromDerivative( d1 );

  chebfun c2( c );
  diff2.apply( c, c2 );

  auto& x = c2.xpoints();
  auto& y = c2.ypoints();
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( y[i], -sin(x[i]), 1e-6 );
    EXPECT_NEAR( d2(x[i]), -sin(x[i]), 1e-4 ); // numeric derivative
    //std::cerr << x[i] << ' ' << y[i] << ' ' << d2(x[i]) << ' ' << -sin(x[i]) << std::endl;
  }
}

TEST(Numeric_ChebOperator_Test, DiffEquationTest)
{
  ChebPlus plus; // y' - y = 0
  plus.add('+',new ChebDiff);
  plus.add('-',new ChebIdentity);
  GSLMatrix L;
  chebfun c( 15, 0.0, 1.0 );
  size_t n = c.n() + 1;

  plus.createMatrix( c.getBase(), L );

  GSLVector rhs(n);
  rhs.zero();
  rhs.set(0, 1);
  double l00 = L.get(0,0);
  L.set(0,0,l00 + 1.0);
  GSLVector y(n);
  L.solve(rhs, y);
  //std::cerr << "y=" << y.get( n - 1 ) << std::endl;
  EXPECT_NEAR(y.get( n - 1 ), 2.718282, 1e-6);
  auto& x = c.xpoints();
  // solution is exp(x)
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( y[i], exp(x[i]), 1e-10 );
    //std::cerr << y[i] << std::endl;
  }
}

TEST(Numeric_ChebOperator_Test, Diff2EquationTest)
{
  ChebPlus plus; // y'' + y = 0
  plus.add('+',new ChebDiff2);
  plus.add('+',new ChebIdentity);
  GSLMatrix L;
  chebfun c( 15, 0.0, 1.0 );
  size_t n = c.n() + 1;

  plus.createMatrix( c.getBase(), L );

  GSLVector rhs(n);
  rhs.zero();

  Cauchy bc(0, 1); // y(0) == 0, y'(0) == 1
  bc.apply(c.getBase(), L, rhs);
  GSLVector y(n);
  L.solve(rhs, y);
  //std::cerr << "y=" << y.get( n - 1 ) << std::endl;
  auto& x = c.xpoints();
  // solution is sin(x)
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( y[i], sin(x[i]), 1e-8 );
    //std::cerr << y[i] << ' ' << sin(x[i]) << std::endl;
  }
}

TEST(Numeric_ChebOperator_Test, SolveTest)
{
  ChebPlus plus; // y'' + y = 0
  plus.add('+',new ChebDiff2);
  plus.add('+',new ChebIdentity);

  chebfun c( 15, 0.0, 1.0 );

  Cauchy bc(1, 0); // y(0) == 1, y'(0) == 0
  plus.solve(c, bc);

  auto& x = c.xpoints();
  // solution is cos(x)
  for(size_t i = 0; i < x.size(); ++i)
  {
    EXPECT_NEAR( c(x[i]), cos(x[i]), 1e-8 );
    //std::cerr << c(x[i]) << ' ' << cos(x[i]) << std::endl;
  }
}

TEST(Numeric_ChebOperator_Test, CreateTest)
{
  auto oper = ChebOperator::create( "diff2 + diff + 1" );
  oper->log();
}

double VPot(double x)
{
    return 0.5*x*x - 0.5;
}

double InitialX(double x)
{
    return -0.5*x*x;
}

TEST(Numeric_ChebOperator_Test, NonLinearTest)
{
    //  beta*(y'' + y'^2) = V - E

//    auto VPot = FunctionFactory::instance().createFitFunction("UserFunction1D");
//    VPot->setAttributeValue("Formula","x^2/2");

    double beta = 0.5;
    chebfun vpot(200, -10, 10);
    vpot.fit(VPot);

    chebfun x(vpot);
    x.fit(InitialX);
    chebfun dx( x );

    ThroughPoint bc(0, 0, 0);

    size_t counter = 0;
    double tol = 1.0;
    while( tol > 0.001 && counter < 10)
    {
        dx.fromDerivative( x );

        ChebCompositeOperator H;
        H.addRight( new ChebTimes(beta) );
        auto derivs = new ChebPlus;
        derivs->add( '+', new ChebDiff2 );
        auto tmp = new ChebCompositeOperator;
        tmp->addRight( new ChebDiff );
        tmp->addRight( new ChebTimes(dx) );
        derivs->add( '+', tmp );
        H.addRight( derivs );

        H.solve(x, vpot, bc);

        chebfun dx1(x);
        dx1.fromDerivative(x);
        dx1.square();

        chebfun dx2(x);
        dx2.fromDerivative2(x);

        dx2 += dx1;
        dx2 *= beta;
        dx2 -= vpot;
        dx2.square();

        tol = dx2.integr();
        std::cerr << "tol=" << tol << std::endl;

        ++counter;

    }
}
