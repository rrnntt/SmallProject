#include "QtAPI/TaskManager.h"
#include "QtAPI/FileTask.h"
#include "QtAPI/TableTask.h"

#include "API/Framework.h"

#include <iostream>

namespace QtAPI
{

TaskManager::TaskManager(const std::string& name):Kernel::DataService<Task>(name)
{
  // default tasks
   add("FileTask",new FileTask());
   add("TableTask",new TableTask());
}

TaskManager& TaskManager::instance()
{
  API::Singleton* s = API::Framework::instance().getSingleton("TaskManager");
  if (s == nullptr)
  {
    TaskManager *f = new TaskManager("TaskManager");
    API::Framework::instance().registerSingleton("TaskManager",f);
    return *f;
  }
  else
  {
    return *static_cast<TaskManager*>(s);
  }
}

} // QtAPI
