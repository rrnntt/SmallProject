#include "gtest/gtest.h"
#include "Numeric/Schrodinger1.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/IFunction.h"

using namespace Numeric;

TEST(Numeric_Schrodinger1_Test, Test)
{
  system("pause");
  auto VPot = FunctionFactory::instance().createFitFunction("UserFunction1D");
  VPot->setAttributeValue("Formula","x^2");
  Schrodinger1 alg;
  alg.setProperty("Beta", 0.5);
  alg.setProperty("StartX", -10.0);
  alg.setProperty("EndX", 10.0);
  alg.setClassProperty("VPot",VPot);
  //alg.get("Chebfun") = "c";
  alg.execute();
}
