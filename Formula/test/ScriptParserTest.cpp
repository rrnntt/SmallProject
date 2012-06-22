#include "gtest/gtest.h"
#include "Formula/ScriptParser.h"

using namespace Formula;

TEST(Formula_ScriptParser_Test, Test)
{
  ScriptParser parser;
  std::string script = "{aaa;bbb;ccc;}; b+1; ccc;";
  parser.parse( script );
  //parser.parse( "ooo; b+1; ccc;" );
}
