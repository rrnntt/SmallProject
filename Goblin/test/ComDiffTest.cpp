#include "gtest/gtest.h"
#include "Goblin/comdiff.h"
#include "Goblin/LineList.h"

#include "Kernel/Framework.h"

#include <iostream>

using namespace Goblin;

TEST(ComDiffTest, Test)
{
  LineList_ptr ll(new LineList);
  //ll->loadAscii(
  std::cerr << Kernel::Framework::instance().binDirectory() << std::endl;
}

