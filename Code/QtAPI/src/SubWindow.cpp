#include "QtAPI/SubWindow.h"

#include <QEvent>

namespace QtAPI
{

SubWindow::SubWindow(QWidget *parent):
QMdiSubWindow(parent)
{
  connect(this,SIGNAL(windowStateChanged(Qt::WindowStates,Qt::WindowStates)),
          this,SLOT(processStateChanged(Qt::WindowStates,Qt::WindowStates)));
}

void SubWindow::processStateChanged(Qt::WindowStates /*oldState*/, Qt::WindowStates newState)
{
  if (newState & Qt::WindowActive)
  {
    emit subWindowBecameActive(this);
  }
}

bool SubWindow::event(QEvent * e)
{
  if (e->type() == QEvent::WindowActivate)
  {
    emit subWindowBecameActive(this);
  }
  return QMdiSubWindow::event(e);
}

void SubWindow::closeEvent(QCloseEvent * e)
{
  emit subWindowClosed(this);
}

} // QtAPI
