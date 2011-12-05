#include "SubWindow.h"

void SubWindow::setTitle(const QString& title)
{
  m_title = title;
  emit setMdiTitle(title);
}
