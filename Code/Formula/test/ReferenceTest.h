#ifndef REFERENCETEST_H
#define REFERENCETEST_H

#include "../Reference.h"
#include "../Scalar.h"
#include "../Namespace.h"
#include "TestSuite.h"

#include <iostream>
#include <sstream>

using namespace Formula;

class ReferenceTest
{
public:
  static void test()
  {
    ReferenceTest p;
    p.testScalar();
  }

  void testScalar()
  {

    Namespace ns;
    ns.addVariable("Scalar","x");
    ns.addVariable("Scalar","y","10");
    ns.addVariable("Ref","r","x");
    Scalar& xx = ns.getVariable("x")->as<Scalar>();
    ns.setVariable("r","17.5");
    std::cerr <<"x="<<xx<<std::endl;
    ns.setVariable("r",*ns.getVariable("y"));
    std::cerr <<"x="<<xx<<std::endl;

  }
};

#endif // REFERENCETEST_H
