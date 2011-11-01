#ifndef NAMESPACETEST_H
#define NAMESPACETEST_H

#include "../Namespace.h"
#include "../Scalar.h"
#include "TestSuite.h"

#include <iostream>

using namespace Formula;

class NamespaceTest
{
public:
  static void test()
  {
    NamespaceTest p;
    p.testNamespace();
  }

  void testNamespace()
  {
    Namespace ns;
    ns.addVariable("Scalar","x");
    ns.addVariable("Scalar","y");
    ns.addVariable("Scalar","z");
    //ns.logVars();
    ns.setVariable("x","10");

    std::set<std::string> vnames = ns.getVarNames();
    std::set<std::string> freenames = ns.getFreeNames();
    std::set<std::string> setnames = ns.getSetVarNames();

    TS_ASSERT_EQUALS(vnames.size(),3);
    TS_ASSERT_EQUALS(freenames.size(),2);
    TS_ASSERT_EQUALS(setnames.size(),1);
    TS_ASSERT_EQUALS(setnames.count("x"),1);
    TS_ASSERT_EQUALS(freenames.count("y"),1);
    TS_ASSERT_EQUALS(freenames.count("z"),1);

    const Scalar& const_s = ns.getVariable("x")->as<Scalar>();

    TS_ASSERT_EQUALS(const_s,10);

    Scalar& s = ns.getVariable("x")->as<Scalar>();
    s.setValue(11);
    TS_ASSERT_EQUALS(ns.getVariable("x")->toString(),"11");
    ns.setVariable("x","12");
    TS_ASSERT_EQUALS(ns.getVariable("x")->toString(),"12");
    double d13 = 13;
    ns.setVariable("x",&d13);
    TS_ASSERT_EQUALS(ns.getVariable("x")->toString(),"13");

  }
};

#endif // NAMESPACETEST_H
