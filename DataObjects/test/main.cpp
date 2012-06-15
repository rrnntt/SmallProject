#include "gtest/gtest.h"
#include "API/TableWorkspace.h"
#include "API/WorkspaceFactory.h"
#include "Kernel/Framework.h"

int main(int argc, char **argv) {
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  Kernel::Framework::instance().openLibrary(
    Kernel::Framework::instance().binDirectory() + "DataObjects.dll"
    );
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

  //API::TableWorkspace

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
