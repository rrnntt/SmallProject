#include "gtest/gtest.h"
#include "Kernel/CommonProperties.h"

#include <iostream>
#include <string>

using namespace Kernel;

TEST(PropertyTest, DoubleProperty) 
{
  DoubleProperty d = 10.11;

  double v = d;

  EXPECT_EQ(v,10.11);
  EXPECT_EQ(double(d),10.11);

  d = 3.14;

  EXPECT_EQ(double(d),3.14);

  d = "1.23";

  EXPECT_EQ(double(d),1.23);

  std::string str = d;

  EXPECT_EQ(str,"1.23");

  Property* p = &d;

  PropertyType<double>* pd = dynamic_cast<PropertyType<double>*>(p);

  EXPECT_TRUE(pd != nullptr);
  EXPECT_EQ(double(*pd),1.23);

  double d1 = *p;
  EXPECT_EQ(d1,1.23);

}

TEST(PropertyTest, IntProperty) 
{
  IntProperty p = 10;
  EXPECT_EQ(int(p),10);

  Property* prop = &p;

  int i = *prop;
  EXPECT_EQ(i,10);
}

TEST(PropertyTest, Property) 
{
  IntProperty i = 10;
  DoubleProperty d = 3.14;

  Property* p = &i;
  std::string str = *p;
  EXPECT_EQ(str,"10");

  p = &d;
  str = *p;
  EXPECT_EQ(str,"3.14");

  d = (int)i;
  EXPECT_EQ(static_cast<double>(d),10.0);

  DoubleProperty d1 = *p;
  EXPECT_EQ(static_cast<double>(d1),10);

  double d2 = *p;
  EXPECT_EQ(d2, 10);

}

