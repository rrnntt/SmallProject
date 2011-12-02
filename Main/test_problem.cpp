#include "Teach/Problem.h"
#include "Teach/Text.h"
#include "API/Framework.h"

#include <iostream>
#include <fstream>

using namespace Teach;

void test_problem()
{
  std::string fname = API::Framework::instance().binDirectory() + "../../../Teach/Text1.txt";
  std::ifstream fil(fname.c_str());
  Problem text;
  text.read(fil);
  std::cerr << text << std::endl;
}
