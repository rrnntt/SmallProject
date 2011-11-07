#include "Formula/EParser.h"

#include <iostream>

using namespace Formula;

void test_parser()
{
  std::string str("Hello");
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

}

