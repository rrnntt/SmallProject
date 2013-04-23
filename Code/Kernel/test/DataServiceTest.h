#ifndef DATASERVICETEST_H
#define DATASERVICETEST_H

#include "../DataService.h"
#include "TestSuite.h"

#include <iostream>
#include <string>

using namespace Kernel;

class DataServiceTest
{
public:
  static void test()
  {
    std::cerr<<"\nDataServiceTest\n";
    DataServiceTest test;
    test.testItWorks();
    test.testAddNotificationWorks();
  }

  class TestData
  {
  public:
    TestData(const std::string& s = ""):str(s){}
    std::string str;
  };

  class TestDataService:public DataService<TestData>
  {
  public:
    TestDataService(const std::string& name):DataService<TestData>(name){}
  };

  void testItWorks()
  {
    std::cerr<<"It works\n";
    TestDataService srvc("Test");
    srvc.add("first",new TestData("First"));
    TS_ASSERT_EQUALS(srvc.size(),1);
    boost::shared_ptr<TestData> dat = srvc.retrieve("first");
    TS_ASSERT(dat);
    TS_ASSERT_EQUALS(dat->str,"First");
  }

  class TestDataObserver: public NotificationObserver
  {
  public:
    TestDataObserver()
    {
      setHandler(this,&TestDataObserver::handlAdd);
    }
    void handlAdd(const DataService<TestData>::AddNotification& nt)
    {
      std::cerr<<"Add notification "<<nt.object_name()<<'\n';
    }
  };

  void testAddNotificationWorks()
  {
    TestDataObserver obs;
    TestDataService srvc("Test");
    srvc.notificationCenter.addObserver(&obs);
    srvc.add("first",new TestData("First"));
  }

};

#endif // DATASERVICETEST_H
