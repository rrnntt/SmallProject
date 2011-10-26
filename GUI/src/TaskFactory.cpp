#include "TaskFactory.h"

#include <iostream>

TaskFactory::TaskFactory():Kernel::DynamicFactory<Task>()
{
}

TaskFactory& TaskFactory::instance()
{
  static TaskFactory factory;
  return factory;
}
