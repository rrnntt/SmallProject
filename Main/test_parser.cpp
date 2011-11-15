#include "Formula/EParser.h"

#include <iostream>

using namespace Formula;

template<class FUN>
void checkMatch(const IParser& parser,FUN fun)
{
  if (parser.hasMatch())
  {
    std::string res = parser.match();
    std::cerr << "Found " << res << ' ' << res.size() << std::endl;
    fun();
  }
  else
  {
    std::cerr << "Not found" << std::endl;
  }

}

void checkMatch(const IParser& parser)
{
  checkMatch(parser,[](){});
}

void test_parser()
{
  std::string str("Hello");

  //-----------------------------------------------------
  CharParser ch('H');
  auto it = ch.match(str.begin(),str.end());

  checkMatch(ch);

  //-----------------------------------------------------
  SeqParser seq;
  seq.addParser(new CharParser('H'));
  seq.addParser(new CharParser('e'));
  seq.addParser(new ListParser(new CharParser("rsl")));
  seq.addParser(new CharParser('o'));

  std::string str1("Helrslo");
  it = seq.match(str1.begin(),str1.end());
  checkMatch(seq);

  //-----------------------------------------------------
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

  it = alt.match(str2.begin(),str2.end());
  checkMatch(alt);

  IParser* alt2 = alt.clone();
  it = alt2->match(str.begin(),str.end());
  checkMatch(*alt2);

  //-----------------------------------------------------
  std::string str3("xyz=  12");
  SeqParser seq3;
  seq3.addParser(new WordParser());
  seq3.addParser(new CharParser(' '),'*');
  seq3.addParser(new CharParser("+-=<>"));
  seq3.addParser(new CharParser(' '),'*');
  seq3.addParser(new WordParser());

  it = seq3.match(str3.begin(),str3.end());

  checkMatch(seq3,[&seq3](){
    std::cerr << "w1=" << seq3.getParser(0)->match() << " w2=" << seq3.getParser(4)->match() << std::endl;
  });

  //-----------------------------------------------------
  str = "x12_3";
  VarNameParser var;
  it = var.match(str.begin(),str.end());

  checkMatch(var);

  //-----------------------------------------------------
  str = "12.34";
  NumberParser num;
  it = num.match(str.begin(),str.end());

  checkMatch(num);

  //-----------------------------------------------------
  str = "x1 = -56";
  SeqParser terms;
  terms.addParser(new TermParser);
  terms.addParser(new CharParser("+-=<> "),'+');
  terms.addParser(new TermParser);

  it = terms.match(str);

  checkMatch(terms,[&terms](){
    std::cerr << "t1=" << terms.getParser(0)->match() << " t2=" << terms.getParser(2)->match() << std::endl;
  });

  //-----------------------------------------------------
  {
    std::string str = "(1+2+3)";
    SeqParser* p = new SeqParser;
    p->addParser(new NumberParser);
    p->addParser(new CharParser('+'));
    p->addParser(new NumberParser);
    p->addParser(new CharParser('+'));
    p->addParser(new NumberParser);
    BracketParser par(p);
    //par.addParser(new TermParser);
    //par.addParser(new CharParser(" "),'*');
    //par.addParser(new BracketParser);
    //par.addParser(new CharParser(" "),'*');
    //par.addParser(new TermParser);

    it = par.match(str);
    checkMatch(par,[&p](){
      std::cerr << p->getParser(0)->match() << ' ' << 
        p->getParser(2)->match() << ' ' << 
        p->getParser(4)->match() << ' ' << std::endl;
    });
  }
  //-----------------------------------------------------
  {
    std::string str = "fun ( x + y )";
    NameBracketParser fun;

    it = fun.match(str);

    checkMatch(fun,[&fun](){
      std::cerr << "t1=" << fun.getParser(0)->match() << " t2=" << fun.getParser(2)->match() << std::endl;
    });
  }

  //-----------------------------------------------------
  {
    std::string str = " - x + y * 18 - (a + (b/c))";
    EParser parser;
    parser.parse(str);
    parser.log();

  }

  //-----------------------------------------------------
  {
    std::string str = " - (x + y )";
    EParser parser;
    //parser.parse(str);
    //parser.log();

  }

  //-----------------------------------------------------
  {
    //std::string str = " sin (x + y ) * 1 + somefun(x,1,2,sin(x))";
    //std::string str = "somefun(x,1,2,sin(x))";
    std::string str = "sin(x)+1";
    EParser parser;
    parser.parse(str);
    parser.log();

  }

  //-----------------------------------------------------
  {
    std::string str = "abc123";
    StringParser parser("abc");
    it = parser.match(str.begin(),str.end());
    checkMatch(parser);
  }

}

void test_eparser()
{
    //std::string str = " sin (x + y ) * 1 + somefun(x,1,2,sin(x))";
    std::string str = "3*(a + b)* c / t + 2*sin(x+-1)";
    //std::string str = "x+-1";
    EParser parser;
    parser.parse(str);
    parser.log();
}
