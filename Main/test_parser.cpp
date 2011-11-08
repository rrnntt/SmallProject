#include "Formula/EParser.h"

#include <iostream>

using namespace Formula;

void test_parser()
{
  std::string str("Hello");

  //-----------------------------------------------------
  CharParser ch('H');
  auto it = ch.match(str.begin(),str.end());

  if (it != str.end())
  {
    std::string res = ch.match();
    std::cerr << "Found " << res << ' ' << res.size() << std::endl;
  }
  else
  {
    std::cerr << "Not found" << std::endl;
  }

  //-----------------------------------------------------
  SeqParser seq;
  seq.addParser(new CharParser('H'));
  seq.addParser(new CharParser('e'));
  seq.addParser(new ListParser(new CharParser("rsl")));
  seq.addParser(new CharParser('o'));

  std::string str1("Helrslo");
  it = seq.match(str1.begin(),str1.end());

  if (seq.hasMatch())
  {
    std::string res = seq.match();
    std::cerr << "Found " << res << ' ' << res.size() << std::endl;
  }
  else
  {
    std::cerr << "Not found" << std::endl;
  }

}

