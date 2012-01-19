#ifndef QtAPI_TASK_H
#define QtAPI_TASK_H

#include "QtAPI/DllExport.h"
#include "Kernel/DataService.h"
#include <QtCore/QObject>

class QMenu;

namespace QtAPI
{

/**
 * Task performed by the application. Tasks define the actions that the application can do.
 * Application can have several background tasks (eg user account check) and a single
 * front task.
 */
class QTAPI_EXPORT Task: public QObject
{
  Q_OBJECT
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
  QMenu* menu() const {return m_menu;}
protected:
  QMenu *m_menu;
};

typedef boost::shared_ptr<Task> Task_ptr;

} // QtAPI

#endif /*QtAPI_TASK_H*/
