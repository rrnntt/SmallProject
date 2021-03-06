#include "gtest/gtest.h"
#include "Kernel/EParser.h"

#include <iostream>
#include <string>

using namespace Kernel;

TEST(EParserTest, Char) 
{
  std::string str("Hello");
  CharParser ch('H');
  auto it = ch.match(str.begin(),str.end());
  EXPECT_TRUE(ch.hasMatch());
  EXPECT_EQ(ch.match(),"H");
}

TEST(EParserTest, NotString) 
{
  std::string str("abc,def");
  NotStringParser parser(",");
  parser.match(str);
  EXPECT_TRUE(parser.hasMatch());
  EXPECT_EQ(parser.match(),"abc");
}

TEST(EParserTest, NotString1) 
{
  std::string str(",abc,,def");
  NotStringParser parser(",,");
  parser.match(str);
  EXPECT_TRUE(parser.hasMatch());
  EXPECT_EQ(parser.match(),",abc");
}

TEST(EParserTest, NotParser) 
{
  std::string str("<stuff>=123");
  NotParser parser("=,;");
  parser.match(str);
  EXPECT_TRUE(parser.hasMatch());
  EXPECT_EQ(parser.match(),"<");

  SeqParser seq;
  seq.addParser(new NotParser("=,;"),'+');
  seq.match(str);
  EXPECT_TRUE(seq.hasMatch());
  EXPECT_EQ(seq.match(),"<stuff>");
}

TEST(EParserTest, SeqList) 
{
  SeqParser seq;
  seq.addParser(new CharParser('H'));
  seq.addParser(new CharParser('e'));
  seq.addParser(new ListParser(new CharParser("rsl")));
  seq.addParser(new CharParser('o'));

  std::string str1("Helrslo");
  seq.match(str1.begin(),str1.end());
  EXPECT_TRUE(seq.hasMatch());
  EXPECT_EQ(seq.match(),"Helrslo");
}

TEST(EParserTest, ListParser) 
{

  auto line = new SeqParser;
  line->addParser( new NotStringParser(";") );
  line->addParser( new CharParser(";") );
  ListParser lines(line, ListParser::OneMany);

  std::string str("aa; bb ; ccc;");
  lines.match(str);
  std::cerr << "size=" << lines.size() << std::endl;
  std::cerr << lines.match() << std::endl;
  size_t n = lines.size();
  for(size_t i = 0; i < n; ++i)
  {
    std::cerr << "=>" << lines.getParser(i)->match() << std::endl;
  }

  //EXPECT_TRUE(seq.hasMatch());
  //EXPECT_EQ(seq.match(),"Helrslo");
}

TEST(EParserTest, Alt) 
{
  std::string str2("Bye");

  SeqParser * seq1 = new SeqParser;
  seq1->addParser(new CharParser('B'));
  seq1->addParser(new CharParser('y'));
  seq1->addParser(new CharParser('e'));

  SeqParser * seq2 = new SeqParser;
  seq2->addParser(new CharParser('H'));
  seq2->addParser(new CharParser('e'));
  seq2->addParser(new ListParser(new CharParser("rsl")));
  seq2->addParser(new CharParser('o'));

  AltParser alt;
  alt.addParser(seq1);
  alt.addParser(seq2);

  alt.match(str2.begin(),str2.end());
  EXPECT_TRUE(alt.hasMatch());
  EXPECT_EQ(alt.match(),"Bye");

  std::string str("Hello");
  IParser* alt2 = alt.clone();
  alt2->match(str.begin(),str.end());
  EXPECT_TRUE(alt2->hasMatch());
  EXPECT_EQ(alt2->match(),"Hello");
  delete alt2;
}

TEST(EParserTest, Word) 
{
  std::string str3("xyz=  12");
  SeqParser seq3;
  seq3.addParser(new WordParser());
  seq3.addParser(new CharParser(' '),'*');
  seq3.addParser(new WordParser());

  seq3.match(str3.begin(),str3.end());

  EXPECT_TRUE(seq3.hasMatch());
  EXPECT_EQ(seq3.getParser(0)->match(),"xyz=");
  EXPECT_EQ(seq3.getParser(2)->match(),"12");
}

TEST(EParserTest, VarName) 
{
  std::string str = "x12_3";
  VarNameParser var;
  var.match(str.begin(),str.end());
  EXPECT_TRUE(var.hasMatch());
  EXPECT_EQ(var.match(),"x12");
}

TEST(EParserTest, Number) 
{
  std::string str = "12.34";
  NumberParser num;
  num.match(str.begin(),str.end());
  EXPECT_TRUE(num.hasMatch());
  EXPECT_EQ(num.match(),"12.34");

  NumberParser num1;
  str = "1e10";
  num1.match(str.begin(),str.end());
  EXPECT_TRUE(num1.hasMatch());
  EXPECT_EQ(num1.match(),"1e10");
}

TEST(EParserTest, Term) 
{
  std::string str = "x1 = -56";
  SeqParser terms;
  terms.addParser(new TermParser);
  terms.addParser(new CharParser("+-=<> "),'+');
  terms.addParser(new TermParser);

  terms.match(str);

  EXPECT_TRUE(terms.hasMatch());
  EXPECT_EQ(terms.getParser(0)->match(),"x1");
  EXPECT_EQ(terms.getParser(2)->match(),"56");
}

TEST(EParserTest, Bracket) 
{
  std::string str = "(1+2+3)";
  SeqParser* p = new SeqParser;
  p->addParser(new NumberParser);
  p->addParser(new CharParser('+'));
  p->addParser(new NumberParser);
  p->addParser(new CharParser('+'));
  p->addParser(new NumberParser);
  BracketParser par(p);

  par.match(str);

  EXPECT_TRUE(par.hasMatch());
  EXPECT_EQ(p->getParser(0)->match(),"1");
  EXPECT_EQ(p->getParser(2)->match(),"2");
  EXPECT_EQ(p->getParser(4)->match(),"3");

}

TEST(EParserTest, BracketSame) 
{
  std::string str = "|1+2+3|";
  SeqParser* p = new SeqParser;
  p->addParser(new NumberParser);
  p->addParser(new CharParser('+'));
  p->addParser(new NumberParser);
  p->addParser(new CharParser('+'));
  p->addParser(new NumberParser);
  BracketParser par("|","|",p);

  par.match(str);

  EXPECT_TRUE(par.hasMatch());
  EXPECT_EQ(p->getParser(0)->match(),"1");
  EXPECT_EQ(p->getParser(2)->match(),"2");
  EXPECT_EQ(p->getParser(4)->match(),"3");

}

TEST(EParserTest, NameBracket) 
{
  std::string str = "fun ( x + y )";
  NameBracketParser fun;

  fun.match(str);

  EXPECT_TRUE(fun.hasMatch());
  EXPECT_EQ(fun.getParser(0)->match(),"fun");
  EXPECT_EQ(fun.getParser(2)->match(),"( x + y )");
}

TEST(EParserTest, SimpleExpression) 
{
  std::string str = " - x + y * 18 - (a + (b/c))";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"-x+y*18-a+b/c");
}

TEST(EParserTest, MinusExpression) 
{
  std::string str = " - (x + y )";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"-(x+y)");
}

TEST(EParserTest, FunctionExpression) 
{
  std::string str = " sin (x + y ) * 1 + somefun(x,1,2,sin(x))";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"sin(x+y)*1+somefun(x,1,2,sin(x))");

  // this one FAILS!
  //system("pause");
  //parser.parse("fun(   )");
  //EXPECT_EQ(parser.str(),"fun");
}

TEST(EParserTest, String) 
{
  std::string str = "abc123";
  StringParser parser("abc");
  parser.match(str.begin(),str.end());
  EXPECT_TRUE(parser.hasMatch());
  EXPECT_EQ(parser.match(),"abc");
}

TEST(EParserTest, UnaryAndBinary) 
{
  std::string str = "x = - 1 + y";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"x=-1+y");
}

TEST(EParserTest, EParserOperators) 
{
  std::vector<std::string> ops;
  ops.push_back(";");
  ops.push_back(",");
  ops.push_back("=");

  // EParser must have at least 1 unary operator :-(
  std::set<std::string> un;
  un.insert("$");

  std::string str = "x = y,z=  +x";
  EParser parser(ops,un);
  parser.parse(str);
  //parser.log();
  EXPECT_EQ(parser.str(),"x=y,z=+x");
  EXPECT_EQ( parser.name(), "," );
}

TEST(EParserTest, EParserFunctions) 
{
  std::string str = "fun(x,y)";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"fun(x,y)");
  parser.parse("sin(x)");
  EXPECT_EQ(parser.str(),"sin(x)");
}

TEST(EParserTest, EParserPower) 
{
  std::string str = " x ^ 2";
  EParser parser;
  parser.parse(str);
  EXPECT_EQ(parser.str(),"x^2");
}

TEST(EParserTest, EParserStr) 
{
  EParser parser;

  parser.parse("a+(b*x)");
  EXPECT_EQ(parser.str(),"a+b*x");

  parser.parse("(a + b) * x");
  EXPECT_EQ(parser.str(),"(a+b)*x");

  parser.parse("fun(a,b,a+b,f(x,y,(z+1)^2))");
  EXPECT_EQ(parser.str(),"fun(a,b,a+b,f(x,y,(z+1)^2))");
}

TEST(EParserTest, EParserUnary) 
{
  EParser parser;

  parser.parse("-a");
  EXPECT_EQ(parser.str(),"-a");

  parser.parse("-abs(x)");
  EXPECT_EQ(parser.str(),"-abs(x)");

  parser.parse("-(x+y)");
  EXPECT_EQ(parser.str(),"-(x+y)");

  parser.parse("-fun(x+y)");
  EXPECT_EQ(parser.str(),"-fun(x+y)");

  parser.parse("-x^2");
  EXPECT_EQ(parser.str(),"-(x^2)");

  parser.parse("-(x^2)");
  EXPECT_EQ(parser.str(),"-(x^2)");

  parser.parse("-x*x");
  EXPECT_EQ(parser.str(),"-(x*x)");

  parser.parse("-x+x");
  EXPECT_EQ(parser.str(),"-x+x");

  parser.parse("-x,x");
  EXPECT_EQ(parser.str(),"-x,x");

  parser.parse("-f(x,x)");
  EXPECT_EQ(parser.str(),"-f(x,x)");

}

TEST(EParserTest, ToList) 
{
  EParser parser;
  parser.parse("a,b,c");
  EXPECT_EQ( parser.name(), "," );

  EParser parser1;
  parser1.parse("abc(x)");
  parser1.toList();
  EXPECT_EQ( parser1.name(), "," );
  EXPECT_EQ( parser1.str(), ",(abc(x))" );
}
