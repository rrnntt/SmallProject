#include "gtest/gtest.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceFactory.h"

int main(int argc, char **argv) {
  //::testing::InitGoogleTest(&argc, argv);
  //return RUN_ALL_TESTS();

  //DataObjects::TableWorkspace
  try
  {
    API::Workspace* ws = API::WorkspaceFactory::instance().create("TableWorkspace");
    std::cerr << "OK" << std::endl;
  }
  catch(...)
  {
    std::cerr << "Fialed" << std::endl;
  }

  return 0;
}
