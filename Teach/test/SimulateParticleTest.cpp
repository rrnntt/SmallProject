#include "gtest/gtest.h"
#include "Teach/SimulateParticle.h"
#include "DataObjects/ChebfunWorkspace.h"

using namespace Teach;

TEST(Teach_SimulateParticle_Test, Test)
{
  SimulateParticle simp;
  simp.execute();
  auto xproj = simp.get("XProj").to<DataObjects::ChebfunWorkspace_sptr>();
  EXPECT_TRUE(xproj);
}
