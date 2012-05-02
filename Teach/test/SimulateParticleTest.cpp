#include "gtest/gtest.h"
#include "Teach/SimulateParticle.h"
#include "DataObjects/ChebfunWorkspace.h"
#include "API/WorkspaceProperty.h"

using namespace Teach;

TEST(Teach_SimulateParticle_Test, Test)
{
  SimulateParticle simp;
  simp.execute();
  auto xproj = simp.get("XProj").as<API::WorkspaceProperty>().to<DataObjects::ChebfunWorkspace>();
  EXPECT_TRUE(xproj);
  size_t n = xproj->nfuns();
}
