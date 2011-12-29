#ifndef VIEWTASK_H
#define VIEWTASK_H

#include "Task.h"
#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QAction>

class ViewTask: public Task
{
  Q_OBJECT
public:
  ViewTask();
  virtual Type type() const{return Background;}
  void setMainWindow(MainWindow* wnd);
private slots:
};

#endif /*VIEWTASK_H*/
