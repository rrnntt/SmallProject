#include "gtest/gtest.h"
#include "Numeric/MakeQuadratureScheme.h"

using namespace Numeric;

TEST(Numeric_MakeQuadratureScheme_Test, Test)
{
  MakeQuadratureScheme alg;
  alg.setProperty("N",10);
  alg.setProperty("Interval","0, 30, inf");
  alg.setProperty("Function","UserFunction1D(Formula=exp(-x^2))");
  alg.setProperty("Quadrature","quad");

  alg.execute();
}
