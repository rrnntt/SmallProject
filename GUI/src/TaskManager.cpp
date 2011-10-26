#include "TaskManager.h"

#include <iostream>

TaskManager::TaskManager(const std::string& name):Kernel::DataService<Task>(name)
{
}

TaskManager& TaskManager::instance()
{
  static TaskManager taskManager("TaskManager");
  return taskManager;
}
