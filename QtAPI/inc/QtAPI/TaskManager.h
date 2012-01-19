#ifndef QtAPI_TASKMANAGER_H
#define QtAPI_TASKMANAGER_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"
#include "Kernel/DataService.h"
#include "API/Singleton.h"

namespace QtAPI
{

class QTAPI_EXPORT TaskManager: public Kernel::DataService<Task>, protected API::Singleton
{
public:
  virtual std::string name() const {return "TaskManager";}
  static TaskManager& instance();
  void setFrontTask(const std::string& taskName);
protected:
  TaskManager(const std::string& name);

};

} // QtAPI

#endif /*TASKMANAGER_H*/