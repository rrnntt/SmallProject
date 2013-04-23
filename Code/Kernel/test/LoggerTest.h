#ifndef LOGGERTEST_H
#define LOGGERTEST_H

#include "../Logger.h"
#include "TestSuite.h"

#include <iostream>

using namespace Kernel;

class LoggerTest
{
public:
  static void test()
  {
    LoggerTest test;
    test.testLoggerWorks();
  }

  void testLoggerWorks()
  {
    Logger& log = Logger::get("Stuff");
    log << "Hello!\n";
  }

};

#endif // LOGGERTEST_H
