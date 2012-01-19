//----------------------------
//  Local includes
//----------------------------
#include "QtAPI/FileTask.h"
#include "QtAPI/TaskFactory.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/Table.h"
#include "QtAPI/TableDialog.h"

//----------------------------
//  Other project includes
//----------------------------
#include "API/WorkspaceFactory.h"
#include "API/WorkspaceManager.h"

//----------------------------
//  Qt includes
//----------------------------
//#include <QtGui/QMdiSubWindow>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>

//----------------------------
//  C++ includes
//----------------------------
#include <iostream>

namespace QtAPI
{

//DECLARE_TASK(FileTask);

FileTask::FileTask()
{
  m_menu = new QMenu("File");

  m_newTable = new QAction("New table", this);
  connect(m_newTable,SIGNAL(triggered()),this,SLOT(newTable()));

  m_loadAsciiTable = new QAction("Load table", this);
  connect(m_loadAsciiTable,SIGNAL(triggered()),this,SLOT(loadAsciiTable()));

  m_menu->addAction(m_newTable);
  m_menu->addAction(m_loadAsciiTable);
}

void FileTask::newTable()
{
  DataObjects::TableWorkspace_ptr ws( dynamic_cast<DataObjects::TableWorkspace*>(
    API::WorkspaceFactory  ::instance().create("TableWorkspace")
    ));
  QString name = "Table_" + QString::number(API::WorkspaceManager::instance().size() + 1);// TODO: not robust
  API::WorkspaceManager::instance().addOrReplace(name.toStdString(),ws);
  ws->addColumn("double","X");
  ws->addColumn("double","Y");
  ws->setRowCount(10);
  SubWindow* wnd = WindowManager::instance().newSubWindow(new Table(ws));
  wnd->setWindowTitle(name);
}

/**
 * Load a TableWorkspace from an ascii file and create a Table for it.
 */
void FileTask::loadAsciiTable()
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

} // QtAPI
