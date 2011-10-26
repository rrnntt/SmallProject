#ifndef TASK_H
#define TASK_H

#include "Kernel/DataService.h"

/**
 * Task performed by the application. Tasks define the actions that the application can do.
 * Application can have several background tasks (eg user account check) and a single
 * front task.
 */
class Task
{
public:
  /**
   * Type of task. Application can have 1 active Front Task
   * and multiple Background tasks.
   */
  enum Type {Background,Front};
  Task(){}
  virtual ~Task(){}
  /// Type of the task
  virtual Type type() const  = 0;
protected:
};

#endif /*TASK_H*/