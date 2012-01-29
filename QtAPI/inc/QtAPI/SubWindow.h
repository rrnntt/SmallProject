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
  SubWindow(QWidget *parent = NULL);
  virtual QMenu* makeContext() const {return nullptr;}
signals:
  void subWindowBecameActive(QtAPI::SubWindow* w);
  void subWindowClosed(QtAPI::SubWindow* w);
protected slots:
  void processStateChanged(Qt::WindowStates oldState, Qt::WindowStates newSTate);
protected:
  bool event(QEvent * e);
  void closeEvent(QCloseEvent * e);
};

} // QtAPI

#endif // QTAPI_SUBWINDOW_H
