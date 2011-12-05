#ifndef TASK_H
#define TASK_H

#include "Kernel/DataService.h"
#include <QtCore/QObject>

class MainWindow;
class QMenu;

/**
 * Task performed by the application. Tasks define the actions that the application can do.
 * Application can have several background tasks (eg user account check) and a single
 * front task.
 */
class Task: public QObject
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
  void setMainWindow(MainWindow* wnd){m_mainWindow = wnd;}
  QMenu* menu() const {return m_menu;}
protected:
  MainWindow* m_mainWindow;
  QMenu *m_menu;
};

#endif /*TASK_H*/