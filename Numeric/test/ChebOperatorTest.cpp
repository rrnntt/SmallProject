#include "gtest/gtest.h"
#include "Numeric/ChebOperator.h"

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
