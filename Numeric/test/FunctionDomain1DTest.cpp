#include "gtest/gtest.h"
#include "Numeric/FunctionDomain1D.h"

#include <iostream>

using namespace Numeric;

TEST(FunctionDomain1DTest, CreateDomainMapTest)
{
  FunctionDomain1DVector domain(2.0, 5.0, 10);
  JointDomain joint;
  joint.addDomain(FunctionDomain_sptr(new FunctionDomain1DVector(0.0, 1.0, 5)));
  joint.addDomain(FunctionDomain_sptr(new FunctionDomain1DVector(1.0, 3.0, 5)));
  joint.addDomain(FunctionDomain_sptr(new FunctionDomain1DVector(3.0, 4.0, 5)));
  joint.addDomain(FunctionDomain_sptr(new FunctionDomain1DVector(4.0, 7.0, 5)));

  DomainMap map = domain.createDomainMap( joint );
  EXPECT_EQ(map.size(), 3);

  EXPECT_EQ( map[0].i2, 1 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[0].domain.get())->startX(), 2.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[0].domain.get())->endX(), 3.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[0].domain.get())->size(), 4 );

  EXPECT_EQ( map[1].i2, 2 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[1].domain.get())->startX(), 3.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[1].domain.get())->endX(), 4.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[1].domain.get())->size(), 4 );

  EXPECT_EQ( map[2].i2, 3 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[2].domain.get())->startX(), 4.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[2].domain.get())->endX(), 5.0 );
  EXPECT_EQ( dynamic_cast<FunctionDomain1D*>(map[2].domain.get())->size(), 4 );

}

