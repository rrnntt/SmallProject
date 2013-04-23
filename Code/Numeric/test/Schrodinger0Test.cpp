#include "gtest/gtest.h"
#include "Numeric/Schrodinger0.h"

using namespace Numeric;

TEST(Numeric_Schrodinger0_Test, Test)
{
  Schrodinger0 alg;
  alg.setProperty("Operator", "-1*diff2+x^2");
  alg.setProperty("StartX", "-1");
  alg.setProperty("EndX", "1");
  alg.setProperty("N", 5);
  //alg.get("Chebfun") = "c";
  //alg.execute();
}
