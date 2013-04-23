#include "gtest/gtest.h"
#include "Formula/ScriptLine.h"
#include "Formula/ScriptModule.h"
#include "Formula/Namespace.h"
#include "Formula/Expression.h"
#include "Formula/Scalar.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace Formula;

TEST(ScriptLineTest,Lines)
{
  Namespace_ptr ns(new Namespace());

  ScriptLineDef defX(ns, "Scalar", "x", "3.14");
  ScriptLineDef defY(ns, "Scalar", "y", "2");
  ScriptLineDef defZ(ns, "Scalar", "z");

  EXPECT_EQ( (double)ns->getVariable("x")->as<Scalar>(), 3.14 );
  EXPECT_EQ( (double)ns->getVariable("y")->as<Scalar>(), 2.0 );
  EXPECT_EQ( (double)ns->getVariable("z")->as<Scalar>(), 0.0 );

  auto expr = Expression_ptr( new Expression(ns,"x + 2*y") );

  ScriptLineSet setZ(ns, "z", expr);
  setZ.eval();

  EXPECT_NEAR( (double)ns->getVariable("z")->as<Scalar>(), 7.14, 1e-14 );

  expr = Expression_ptr( new Expression(ns,"sin(x - 1)") );
  ScriptLineSet setZ1(ns, "z", expr);
  setZ1.eval();

  EXPECT_NEAR( (double)ns->getVariable("z")->as<Scalar>(), 0.8423304316366457, 1e-14 );

  ScriptLineIf ifl(ns);
  auto cond = Expression_ptr( new Expression(ns,"x > y") );
  auto trueExpr = Expression_ptr( new Expression(ns,"2*x") );
  auto falseExpr = Expression_ptr( new Expression(ns,"2*y") );
  auto trueLine = ScriptLine_ptr( new ScriptLineSet(ns, "z", trueExpr) );
  auto falseLine = ScriptLine_ptr( new ScriptLineSet(ns, "z", falseExpr) );
  ifl.addCondition(cond, trueLine);
  ifl.addDefault(falseLine);
  ifl.eval();
  EXPECT_NEAR( (double)ns->getVariable("z")->as<Scalar>(), 6.28, 1e-14 );


}

TEST(ScriptLineTest,Block)
{
  //Namespace_ptr ns(new Namespace());

  ScriptLineBlock block;
  block.addScript("Scalar x=3.14; Scalar y=2; Scalar z;");
  block.addScript("z:=y; x := 2*z;sin(3.0);");
  block.eval();

  block.getLocalNamespace().logVars();
  EXPECT_NEAR( (double)block.getLocalNamespace().getVariable("x")->as<Scalar>(), 4.0, 1e-14 );
  EXPECT_NEAR( (double)block.getLocalNamespace().getVariable("y")->as<Scalar>(), 2.0, 1e-14 );
  EXPECT_NEAR( (double)block.getLocalNamespace().getVariable("z")->as<Scalar>(), 2.0, 1e-14 );

}