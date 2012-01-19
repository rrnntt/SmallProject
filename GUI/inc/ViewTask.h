#ifndef GUI_VIEWTASK_H
#define GUI_VIEWTASK_H

#include "QtAPI/Task.h"

#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QAction>

class MainWindow;

class ViewTask: public QtAPI::Task
{
  Q_OBJECT
public:
  ViewTask();
  virtual Type type() const{return Background;}
  void setMainWindow(MainWindow* wnd);
private:
  MainWindow* m_mainWindow;
};

#endif /*GUI_VIEWTASK_H*/
