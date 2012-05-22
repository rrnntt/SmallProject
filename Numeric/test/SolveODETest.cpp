#include "gtest/gtest.h"
#include "Numeric/SolveODE.h"

using namespace Numeric;

TEST(Numeric_SolveODE_Test, Test)
{
  SolveODE algo;
  algo.get("Operator") = "diff2+1";
  algo.get("BoundaryConditions") = "Cauchy";
  algo.get("BoundaryValues") = "0,1";

}
