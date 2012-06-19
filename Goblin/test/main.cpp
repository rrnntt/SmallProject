#include "gtest/gtest.h"
#include "Kernel/Framework.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  return RUN_ALL_TESTS();
}
