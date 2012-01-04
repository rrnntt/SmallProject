#include "gtest/gtest.h"
#include "API/WorkspaceProperty.h"
#include "API/WorkspaceFactory.h"

#include <iostream>
#include <string>

using namespace Kernel;
using namespace API;

class DummyWorkspace: public Workspace
{
public:
  double value;
  DummyWorkspace():value(0){}
};

TEST(WorkspacePropertyTest, Property)
{
  WorkspaceProperty p(Property::Output);
  p = "WorkspacePropertyTest_1";

  std::cerr << "Property " << p << std::endl;

  boost::shared_ptr<DummyWorkspace> dummy(new DummyWorkspace);

  p = dummy;

  //auto dws = p.to_s< boost::shared_ptr<DummyWorkspace> >();


}

