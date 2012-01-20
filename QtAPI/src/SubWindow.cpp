#include "QtAPI/SubWindow.h"

#include <QEvent>

namespace QtAPI
{

bool SubWindow::event(QEvent * e)
{
  if (e->type() == QEvent::WindowActivate)
  {
    emit subWindowBecameActive(this);
  }
  return QMdiSubWindow::event(e);
}
} // QtAPI
