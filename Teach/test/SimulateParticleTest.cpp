#include "gtest/gtest.h"
#include "Teach/SimulateParticle.h"
#include "Numeric/ChebfunWorkspace.h"

using namespace Teach;

TEST(Teach_SimulateParticle_Test, Test)
{
  SimulateParticle simp;
  simp.execute();
  Numeric::ChebfunWorkspace_sptr xproj = simp.getClass("XProj");
  EXPECT_TRUE(xproj);
  size_t n = xproj->fun().nfuns();
}
