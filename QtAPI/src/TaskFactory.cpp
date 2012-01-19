#include "QtAPI/TaskFactory.h"
#include "API/Framework.h"

#include <iostream>

namespace QtAPI
{

TaskFactory::TaskFactory():Kernel::DynamicFactory<Task>()
{
}

TaskFactory& TaskFactory::instance()
{
  API::Singleton* s = API::Framework::instance().getSingleton("TaskFactory");
  if (s == nullptr)
  {
    TaskFactory *f = new TaskFactory();
    API::Framework::instance().registerSingleton("TaskFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<TaskFactory*>(s);
  }
}

} // QtAPI
