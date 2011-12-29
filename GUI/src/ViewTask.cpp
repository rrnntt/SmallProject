//----------------------------
//  Local includes
//----------------------------
#include "ViewTask.h"
#include "TaskFactory.h"
#include "MainWindow.h"
#include "AlgorithmExplorer.h"

//----------------------------
//  Other project includes
//----------------------------

//----------------------------
//  Qt includes
//----------------------------
#include <QtGui/QMdiSubWindow>

//----------------------------
//  C++ includes
//----------------------------
#include <iostream>

//DECLARE_TASK(ViewTask);

ViewTask::ViewTask()
{
  m_menu = new QMenu("View");
}

void ViewTask::setMainWindow(MainWindow* wnd)
{
  Task::setMainWindow(wnd);
  m_menu->addAction(m_mainWindow->m_algorithmExplorer->toggleViewAction());
}
