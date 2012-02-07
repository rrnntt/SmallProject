#include "QtAPI/TaskManager.h"
#include "QtAPI/FileTask.h"
#include "QtAPI/TableTask.h"
#include "QtAPI/PlotTask.h"
#include "QtAPI/TaskFactory.h"

#include "API/Framework.h"

#include <iostream>

namespace QtAPI
{

TaskManager::TaskManager(const std::string& name):Kernel::DataService<Task>(name)
{
  // default tasks
   add("FileTask",new FileTask());
   add("TableTask",new TableTask());
   add("PlotTask",new PlotTask());
   m_to_be_registered.insert("GoblinTask");
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
    TaskManager *f = static_cast<TaskManager*>(s);
    f->registerDelayed();
    return *f;
  }
}

void TaskManager::delayedRegister(const std::string& taskType)
{
  instance().m_to_be_registered.insert(taskType);
}

void TaskManager::registerDelayed()
{
  std::for_each(m_to_be_registered.begin(),m_to_be_registered.end(),[this](const std::string& taskType){
    try
    {
      add(taskType,TaskFactory::instance().create(taskType));
    }
    catch(...)
    {}
  });
  m_to_be_registered.clear();
}

} // QtAPI
