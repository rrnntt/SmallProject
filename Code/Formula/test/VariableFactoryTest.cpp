#include "Formula/VariableFactory.h"
#include "DummyVar.h"
#include "gtest/gtest.h"

using namespace Formula;
DECLARE_VARIABLE(DummyVar);

TEST(VariableFactoryTest, Negative) {
  int i = 3;
  EXPECT_EQ(3, i);
  EXPECT_TRUE(i == 3);
  Variable* var = VariableFactory::instance().create("DummyVar");
  EXPECT_TRUE(var != NULL);
  EXPECT_EQ(var->type(),"DummyVar");
  ((DummyVar*)var)->stuff = 25;
  EXPECT_EQ(((DummyVar*)var)->stuff,25);
  const DummyVar& dvar = dynamic_cast<const DummyVar&>((*var)());
  EXPECT_EQ(((DummyVar*)var)->stuff,dvar.stuff);
}

