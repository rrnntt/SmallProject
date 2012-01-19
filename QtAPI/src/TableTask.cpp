#include "QtAPI/TableTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/SubWindow.h"

#include <QtGui/QAction>

namespace QtAPI
{

TableTask::TableTask()
{
  m_showTableDialog = new QAction("Set values...",this);
  connect(m_showTableDialog,SIGNAL(triggered()),this,SLOT(showTableDialog()));
  addAction("SetValues",m_showTableDialog);
}

QMenu* TableTask::menu(SubWindow* w) const
{
  if (!w) return nullptr;
  Table* table = qobject_cast<Table*>(w->widget());
  if (!table) return nullptr;
  m_table = table;
  QMenu* theMenu = new QMenu("Table");
  theMenu->addAction(m_showTableDialog);
  return theMenu;
}

void TableTask::showTableDialog()
{
  if (!m_table) return;
  m_table->showTableDialog();
}

} // namespace QtAPI
