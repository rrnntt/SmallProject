#ifndef QtAPI_TASKFACTORY_H
#define QtAPI_TASKFACTORY_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"
#include "QtAPI/TaskManager.h"
#include "Kernel/DynamicFactory.h"
#include "API/Singleton.h"

namespace QtAPI
{

class QTAPI_EXPORT TaskFactory: public Kernel::DynamicFactory<Task>, protected API::Singleton
{
public:
  TaskFactory();
  virtual std::string name() const {return "TaskFactory";}
  static TaskFactory& instance();
protected:
};

} // QtAPI

#define DECLARE_TASK(type) \
int declare_task_##type( (\
  QtAPI::TaskFactory::instance().subscribe<type>(#type),\
  QtAPI::TaskManager::delayedRegister(#type),\
  0));

#endif /*QtAPI_TASKFACTORY_H*/