//----------------------------
//  Local includes
//----------------------------
#include "ViewTask.h"
#include "MainWindow.h"
#include "AlgorithmExplorer.h"
#include "WorkspaceExplorer.h"

#include "QtAPI/TaskFactory.h"
#include "QtAPI/WindowManager.h"

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
  m_mainWindow = wnd;
  m_menu->addAction(m_mainWindow->m_algorithmExplorer->toggleViewAction());
  m_menu->addAction(m_mainWindow->m_workspaceExplorer->toggleViewAction());
}