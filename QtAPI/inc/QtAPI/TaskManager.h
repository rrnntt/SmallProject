#ifndef QtAPI_TASKMANAGER_H
#define QtAPI_TASKMANAGER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"
#include "Kernel/DataService.h"
#include "Kernel/Singleton.h"

#include <set>

namespace QtAPI
{

class QTAPI_EXPORT TaskManager: public Kernel::DataService<Task>, protected Kernel::Singleton
{
public:
  virtual std::string name() const {return "TaskManager";}
  static TaskManager& instance();
  static void delayedRegister(const std::string& taskType);
  void setFrontTask(const std::string& taskName);
  template<class T>
  boost::shared_ptr<T> getAs(const std::string& tname) const;
  void errorMessage(const std::string& mess) const;
protected:
  TaskManager(const std::string& name);
  void registerDelayed();
  mutable std::set<std::string> m_to_be_registered;
};

template<class T>
boost::shared_ptr<T> TaskManager::getAs(const std::string& tname) const
{
  boost::shared_ptr<T> task = boost::dynamic_pointer_cast<T>(retrieve(tname));
  return task;
}

} // QtAPI

//#define DECLARE_TASK(type) \
//int declare_task_##type( (QtAPI::TaskManager::instance().add(#type,new type()),0));

#endif /*TASKMANAGER_H*/