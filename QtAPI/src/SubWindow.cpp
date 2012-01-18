#include "QtAPI/SubWindow.h"

namespace QtAPI
{

void SubWindow::setTitle(const QString& title)
{
  m_title = title;
  emit setMdiTitle(title);
}

} // QtAPI
