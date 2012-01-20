#ifndef QTAPI_SUBWINDOW_H
#define QTAPI_SUBWINDOW_H

#include "DllExport.h"

#include <QtGui/QMdiSubWindow>

class QMenu;

namespace QtAPI
{

class QTAPI_EXPORT SubWindow: public QMdiSubWindow
{
  Q_OBJECT
public:
  SubWindow(QWidget *parent = NULL):QMdiSubWindow(parent){}
  virtual QMenu* makeContext() const {return nullptr;}
signals:
  void subWindowBecameActive(QtAPI::SubWindow* w);
protected:
  bool SubWindow::event(QEvent * e);
};

} // QtAPI

#endif // QTAPI_SUBWINDOW_H
