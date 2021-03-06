#include "gtest/gtest.h"
#include "Formula/ScriptParser.h"

using namespace Formula;

TEST(Formula_ScriptParser_Test, VarTest)
{
  std::string script = "SomeType   myVar  \t";
  VarParser p;
  p.match( script );
  EXPECT_EQ( p.type()->match(), "SomeType" );
  EXPECT_EQ( p.var()->match(), "myVar" );
  EXPECT_TRUE( p.value() == nullptr );
}

TEST(Formula_ScriptParser_Test, VarValueTest)
{
  std::string script = "SomeType   myVar  = jnl8987{}!!\t ";
  VarParser p;
  p.match( script );
  EXPECT_EQ( p.type()->match(), "SomeType" );
  EXPECT_EQ( p.var()->match(), "myVar" );
  EXPECT_TRUE( p.value() != nullptr );
  EXPECT_EQ( p.value()->match(), "jnl8987{}!!\t ");
}

TEST(Formula_ScriptParser_Test, IfParserTest)
{
  std::string script = "if  ( x >= 12  ) {do {something}} ";
  IfParser ifp;
  ifp.match(script);
  EXPECT_TRUE( ifp.hasMatch() );
  std::string cond(ifp.condition()->getInnerStart(), ifp.condition()->getInnerEnd());
  EXPECT_EQ( cond, " x >= 12  " );
  std::string trueBlock(ifp.trueBlock()->getInnerStart(), ifp.trueBlock()->getInnerEnd());
  EXPECT_EQ( trueBlock, "do {something}" );
  EXPECT_TRUE( ifp.elseBlock() == nullptr );
}

TEST(Formula_ScriptParser_Test, IfParserElseTest)
{
  std::string script = "if  ( x >= 12  ) {do {something}} else  {  do  something else }";
  IfParser ifp;
  ifp.match(script);
  EXPECT_TRUE( ifp.hasMatch() );
  std::string cond(ifp.condition()->getInnerStart(), ifp.condition()->getInnerEnd());
  EXPECT_EQ( cond, " x >= 12  " );
  std::string trueBlock(ifp.trueBlock()->getInnerStart(), ifp.trueBlock()->getInnerEnd());
  EXPECT_EQ( trueBlock, "do {something}" );
  EXPECT_TRUE( ifp.elseBlock() != nullptr );
  std::string elseBlock(ifp.elseBlock()->getInnerStart(), ifp.elseBlock()->getInnerEnd());
  EXPECT_EQ( elseBlock, "  do  something else ");
}

TEST(Formula_ScriptParser_Test, Test)
{
  ScriptParser parser;
  std::string script = "{aaa;bbb;ccc;}; b+1; ccc; ";
  //system("pause");
  parser.parse( script );
  //parser.parse( "ooo; b+1; ccc;" );
  EXPECT_EQ( parser.size(), 3 );
  EXPECT_TRUE( parser.line(0)->block() );
  EXPECT_FALSE( parser.line(1)->block() );
  EXPECT_FALSE( parser.line(2)->block() );
  EXPECT_TRUE( parser.line(1)->expression() );
  EXPECT_TRUE( parser.line(2)->expression() );

}

TEST(Formula_ScriptParser_Test, Parser)
{
  ScriptParser parser;
  std::string script = "Scalar x = 1; Scalar y; if ( x > y) {y = x; 2*2};";
  parser.parse( script );
  EXPECT_EQ( parser.size(), 3 );
  for(size_t i = 0; i < parser.size(); ++i)
  {
    std::cerr << parser.line(i)->match() << std::endl;
  }
  auto defx = parser.line(0)->defVar();
  EXPECT_TRUE( defx );
  EXPECT_EQ( defx->type()->match(), "Scalar" );
  EXPECT_EQ( defx->var()->match(), "x" );
  EXPECT_EQ( defx->value()->match(), "1" );

  defx = parser.line(1)->defVar();
  EXPECT_TRUE( defx );
  EXPECT_EQ( defx->type()->match(), "Scalar" );
  EXPECT_EQ( defx->var()->match(), "y" );
  EXPECT_FALSE( defx->value() );

  auto ifp = parser.line(2)->ifBlock();
  EXPECT_TRUE( ifp );
  EXPECT_EQ( ifp->condition()->match(), "( x > y)" );
  EXPECT_EQ( ifp->trueBlock()->match(), "{y = x; 2*2}" );
  EXPECT_FALSE( ifp->elseBlock() );
}
