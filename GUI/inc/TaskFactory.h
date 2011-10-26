#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include "Kernel/DynamicFactory.h"
#include "Task.h"

class TaskFactory: public Kernel::DynamicFactory<Task>
{
public:
  TaskFactory();
	static TaskFactory& instance();
protected:
};

#define DECLARE_TASK(type) \
int declare_task_##type( (TaskFactory::instance().subscribe<type>(#type),0));

#endif /*TASKFACTORY_H*/