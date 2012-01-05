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

  boost::shared_ptr<DummyWorkspace> dummy(new DummyWorkspace);

  p = dummy;

  auto dws = p.to< DummyWorkspace >();

  EXPECT_EQ( dws->name(), "WorkspacePropertyTest_1");
  EXPECT_EQ( static_cast<std::string>(p), "WorkspacePropertyTest_1");

  dws->value = 10.0;
  
  EXPECT_EQ(dws->value, 10.0);
}

