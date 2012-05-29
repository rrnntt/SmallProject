#include "gtest/gtest.h"
#include "Numeric/Schrodinger0.h"

using namespace Numeric;

TEST(Numeric_Schrodinger0_Test, Test)
{
  Schrodinger0 alg;
  alg.get("Operator") = "-1*diff2+x^2";
  alg.get("StartX") = "-1";
  alg.get("EndX") = "1";
  alg.get("N") = "5";
  //alg.get("Chebfun") = "c";
  alg.execute();
}
