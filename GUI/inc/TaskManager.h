#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include "Kernel/DataService.h"

class TaskManager: public Kernel::DataService<Task>
{
public:
	static TaskManager& instance();
  void setFrontTask(const std::string& taskName);
protected:
  TaskManager(const std::string& name);

};

#endif /*TASKMANAGER_H*/