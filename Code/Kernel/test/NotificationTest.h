#ifndef NOTIFICATIONTEST_H
#define NOTIFICATIONTEST_H

#include "../Notification.h"
#include "TestSuite.h"

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

using namespace Kernel;

class Not1: public Notification
{
public:
  Not1(){}
  Not1(const std::string& msg):mess(msg){}
  std::string mess;
};

class Obs: public NotificationObserver
{
public:
  virtual void handl(const Not1& nt)
  {
    std::cerr<<"notification "<<nt.mess<<'\n';
  }
};

class NotificationTest
{
public:
  static void test()
  {
    NotificationTest test;
    test.testConcept();
    test.testNotificationCentre();
  }

  void testConcept()
  {
    Notification nt;
    const std::type_info& in = typeid(nt);
    const std::type_info& in1 = typeid(Not1);
    std::cerr<<in.name()<<' '<<in1.name()<<' '<< (in == in1) <<'\n';

    std::vector<const std::type_info*> infos;
    infos.push_back(&typeid(Notification));

    Not1* nt1 = new Not1;
    nt1->mess = "What a mess";
    boost::shared_ptr<Notification> snt1(nt1);

    Obs obs;
    obs.setHandler(&obs,&Obs::handl);
    obs.callHandler(snt1);
  }

  void testNotificationCentre()
  {
    std::cerr<<"\ntestNotificationCentre\n";
    Obs obs;
    obs.setHandler(&obs,&Obs::handl);

    NotificationCenter nc;
    nc.addObserver(&obs);

    nc.send(new Not1("It works!"));
  }
};

#endif // NOTIFICATIONTEST_H
