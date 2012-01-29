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
  virtual QMenu* menu(QtAPI::SubWindow* w = nullptr) const{return m_menu;}
  virtual int menuOrder() const {return 2;}
  void setMainWindow(MainWindow* wnd);
private:
  QMenu* m_menu;
  MainWindow* m_mainWindow;
};

#endif /*GUI_VIEWTASK_H*/
