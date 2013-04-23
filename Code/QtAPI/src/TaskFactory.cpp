#include "QtAPI/TaskFactory.h"
#include "Kernel/Framework.h"

#include <iostream>

namespace QtAPI
{

TaskFactory::TaskFactory():Kernel::DynamicFactory<Task>()
{
}

TaskFactory& TaskFactory::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("TaskFactory");
  if (s == nullptr)
  {
    TaskFactory *f = new TaskFactory();
    Kernel::Framework::instance().registerSingleton("TaskFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<TaskFactory*>(s);
  }
}

} // QtAPI
