#include "gtest/gtest.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceFactory.h"
#include "API/Framework.h"

int main(int argc, char **argv) {
  API::Framework::instance().parseCommandLine(argc,argv);
  API::Framework::instance().openLibrary(
    API::Framework::instance().binDirectory() + "DataObjects.dll"
    );
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

  //DataObjects::TableWorkspace

  try
  {
    API::Workspace* ws = API::WorkspaceFactory::instance().create("TableWorkspace");
    std::cerr << "OK" << std::endl;
  }
  catch(std::exception& e)
  {
    std::cerr << "Fialed " << e.what() << std::endl;
  }

  return 0;
}
