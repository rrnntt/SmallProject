#include "FileTask.h"
#include "TaskFactory.h"
#include "MainWindow.h"
#include "Table.h"
#include "API/WorkspaceFactory.h"
#include "API/WorkspaceManager.h"

#include <QtGui/QMdiSubWindow>

#include <iostream>

DECLARE_TASK(FileTask);

FileTask::FileTask()
{
  m_menu = new QMenu("File");
  m_newTable = new QAction("New table", this);
  connect(m_newTable,SIGNAL(triggered()),this,SLOT(newTable()));
  m_menu->addAction(m_newTable);
}

void FileTask::newTable()
{
  DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
    API::WorkspaceFactory  ::instance().create("TableWorkspace")
    ));
  QString name = "Table_" + QString::number(API::WorkspaceManager::instance().size() + 1);
  API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
  ws->addColumn("double","X");
  ws->addColumn("double","Y");
  ws->setRowCount(10);
  QMdiSubWindow* wnd = m_mainWindow->newSubWindow(new Table(ws,m_mainWindow));
  wnd->setWindowTitle(name);
}
