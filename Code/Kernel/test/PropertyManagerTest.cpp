#include "gtest/gtest.h"
#include "Kernel/PropertyClass.h"
#include "Kernel/PropertyManager.h"
#include "Kernel/PropertyClassFactory.h"
#include "Kernel/Singleton.h"
#include "Kernel/Framework.h"

#include <boost/lexical_cast.hpp>
#include <stdexcept>

using namespace Kernel;

class TestClass: public PropertyClass
{
public:
  TestClass():member(1.0){}
  std::string asString() const {return boost::lexical_cast<std::string>(member);}
  double member;
};

class TestClassFactory: public PropertyClassFactory, public Singleton
{
public:
  std::string name() const {return "TestClass";}
  boost::shared_ptr<PropertyClass> createProperty(const std::string& value) const
  {
    auto tc = new TestClass;
    tc->member = boost::lexical_cast<double>( value );
    return boost::shared_ptr<PropertyClass>( tc );
  }
};

TEST(Kernel_PropertyManager_Test, SimpleTypes)
{
  PropertyManager mgr;
  mgr.declareDouble("x", 3.14);
  mgr.declareInt("i", 31);
  mgr.declareBool("b", true);
  mgr.declareString("s", "hello");

  double x = mgr.get("x");
  EXPECT_EQ( x, 3.14 );

  int i = mgr.get("i");
  EXPECT_EQ( i, 31 );

  bool b = mgr.get("b");
  EXPECT_EQ( b, true );

  std::string s = mgr.get("s");
  EXPECT_EQ( s, "hello" );

  s = (std::string&)mgr.get("x");
  EXPECT_EQ( s, "3.14" );

  mgr.setProperty("x",2.73);
  x = mgr.get("x");
  EXPECT_EQ( x, 2.73 );

  // wrong argument type - int
  EXPECT_THROW( mgr.setProperty("x",2), std::runtime_error );

}

TEST(Kernel_PropertyManager_Test, ClassProperty)
{
  auto factory = new TestClassFactory;
  Framework::instance().registerSingleton("TestClass", factory );
  PropertyManager mgr;
  mgr.declareClass( "c", factory );

  auto tc = new TestClass;
  mgr.setProperty( "c", tc );

  std::string value = mgr.get("c");
  std::cerr << value << std::endl;

  mgr.setProperty("c", "15.3");
  value = (std::string)mgr.get("c");
  std::cerr << value << std::endl;
  
}
