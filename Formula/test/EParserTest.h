#ifndef EPARSERTEST_H
#define EPARSERTEST_H

#include "../EParser.h"
#include "TestSuite.h"

#include <iostream>

class TestEParser
{
public:
  static void test()
  {
    TestEParser p;
    p.testParser();
  }

  void testParser()
  {
    Formula::EParser p;
  }


};

#endif // EPARSERTEST_H
