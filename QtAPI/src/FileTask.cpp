//----------------------------
//  Local includes
//----------------------------
#include "QtAPI/FileTask.h"
#include "QtAPI/TaskFactory.h"
#include "QtAPI/WindowManager.h"
#include "QtAPI/TableTask.h"
#include "QtAPI/TaskManager.h"

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
  boost::shared_ptr<TableTask> task = boost::dynamic_pointer_cast<TableTask>(TaskManager::instance().retrieve("Table"));
  if (!task) return;
  task->newTable();
}

/**
 * Load a TableWorkspace from an ascii file and create a Table for it.
 */
void FileTask::loadAsciiTable()
{
  boost::shared_ptr<TableTask> task = boost::dynamic_pointer_cast<TableTask>(TaskManager::instance().retrieve("Table"));
  if (!task) return;
  task->loadAsciiTable();
}

} // QtAPI
