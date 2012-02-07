#ifndef QtAPI_TASK_H
#define QtAPI_TASK_H

#include "QtAPI/DllExport.h"
#include "Kernel/DataService.h"
#include <QtCore/QObject>
#include <QtCore/QHash>

class QMenu;
class QAction;

namespace QtAPI
{

class SubWindow;

/// The order
const int customMenuOder = 10;

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
  virtual QMenu* menu(SubWindow* w = nullptr) const = 0;
  /// Returns order of the task in the menubar, doesn't have to be unique.
  /// Tasks are sorted in their menuOrder before adding to the menubar.
  /// Tasks with smaller order go first in the menubar.
  /// Tasks of subwindows should return customMenuOder constant.
  virtual int menuOrder() const = 0;

  QAction* getAction(const QString& name) const;
protected:
  void addAction(const QString& name, QAction* action);
  void errorMessage(const std::string& mess) const;
  std::string getInput(const std::string& mess)const;
  QHash<QString, QAction*> m_actions;
  int m_menuOrder;
};

typedef boost::shared_ptr<Task> Task_ptr;

} // QtAPI

#endif /*QtAPI_TASK_H*/
