#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include "Task.h"
#include "Kernel/DynamicFactory.h"
#include "API/Singleton.h"

class TaskFactory: public Kernel::DynamicFactory<Task>, protected API::Singleton
{
public:
  TaskFactory();
  virtual std::string name() const {return "TaskFactory";}
  static TaskFactory& instance();
protected:
};

#define DECLARE_TASK(type) \
int declare_task_##type( (TaskFactory::instance().subscribe<type>(#type),0));

#endif /*TASKFACTORY_H*/