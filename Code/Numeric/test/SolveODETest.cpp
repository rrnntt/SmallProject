#include "gtest/gtest.h"
#include "Numeric/SolveODE.h"

using namespace Numeric;

TEST(Numeric_SolveODE_Test, Test)
{
  SolveODE algo;
  algo.setProperty("Operator", "diff2+1");
  algo.setProperty("BoundaryConditions", "Cauchy");
  algo.setProperty("BoundaryValues", "0,1");

}
