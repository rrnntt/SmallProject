#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include "Kernel/DataService.h"
#include "API/Singleton.h"

class TaskManager: public Kernel::DataService<Task>, protected API::Singleton
{
public:
  virtual std::string name() const {return "TaskManager";}
  static TaskManager& instance();
  void setFrontTask(const std::string& taskName);
protected:
  TaskManager(const std::string& name);

};

#endif /*TASKMANAGER_H*/