#include "gtest/gtest.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceFactory.h"

int main(int argc, char **argv) {
  //::testing::InitGoogleTest(&argc, argv);
  //return RUN_ALL_TESTS();

  //DataObjects::TableWorkspace
  API::Workspace* ws = API::WorkspaceFactory::instance().create("TableWorkspace");
  if (ws)
  {
    std::cerr << "OK" << std::endl;
  }
  else
  {
    std::cerr << "Fialed" << std::endl;
  }

  return 0;
}
