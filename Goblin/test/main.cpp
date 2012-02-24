#include "gtest/gtest.h"
#include "API/Framework.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  API::Framework::instance().parseCommandLine(argc,argv);
  return RUN_ALL_TESTS();
}
