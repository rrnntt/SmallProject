#include "QtAPI/TableTask.h"
#include "QtAPI/Table.h"
#include "QtAPI/SubWindow.h"
#include "QtAPI/WindowManager.h"

#include "API/WorkspaceFactory.h"

#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>

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

QPointer<Table> TableTask::newTable() const
{
  DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
    API::WorkspaceFactory  ::instance().create("TableWorkspace")
    ));
  QString name = "Table_" + QString::number(API::WorkspaceManager::instance().size() + 1);// TODO: not robust
  API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
  ws->addColumn("double","X");
  ws->addColumn("double","Y");
  ws->setRowCount(10);
  Table* table = new Table(ws);
  SubWindow* wnd = WindowManager::instance().newSubWindow(table);
  wnd->setWindowTitle(name);
  return table;
}

/**
 * Load a TableWorkspace from an ascii file and create a Table for it.
 */
void TableTask::loadAsciiTable()const
{
  QString fileName = QFileDialog::getOpenFileName(nullptr,"Open a table");
  if (!fileName.isEmpty())
  {
    QFileInfo file(fileName);
    QString name = file.baseName();
    DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
      API::WorkspaceFactory::instance().create("TableWorkspace")  ));
    ws->loadAscii(fileName.toStdString());
    API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
    SubWindow* wnd = WindowManager::instance().newSubWindow(new Table(ws));
    wnd->setWindowTitle(name);
  }
}



} // namespace QtAPI
