#include "gtest/gtest.h"
#include "Numeric/MakeQuadratureScheme.h"
#include "Numeric/Constants.h"

#include "API/TableWorkspace.h"

using namespace Numeric;

TEST(Numeric_MakeQuadratureScheme_Test, Test)
{
  MakeQuadratureScheme alg;
  alg.setProperty("N",5);
  alg.setProperty("Interval","0, 12");
  alg.setProperty("Function","UserFunction1D(Formula=exp(-x^2))");
  //alg.setProperty("Quadrature","quad");

  alg.execute();
  API::TableWorkspace_ptr tws = alg.getClass("Quadrature");

  EXPECT_EQ( tws->columnCount(), 7 );
  EXPECT_EQ( tws->rowCount(), 5 );

  const size_t n = tws->rowCount();
  auto& r = tws->getDoubleData("r");
  auto& w = tws->getDoubleData("w");
  double d0 = 0;
  double d2 = 0;
  for(size_t i = 0; i < n; ++i)
  {
    double x = r[i];
    d0 += w[i];
    d2 += x*x * w[i];
  }

  EXPECT_NEAR( d0, sqrt(pi)/2, 1e-15 );
  EXPECT_NEAR( d2, sqrt(pi)/4, 1e-15 );
}
