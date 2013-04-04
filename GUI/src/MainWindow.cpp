#include "MainWindow.h"
#include "ViewTask.h"
#include "AlgorithmExplorer.h"
#include "WorkspaceExplorer.h"
#include "FitBrowser.h"

#include "Kernel/Framework.h"

#include "QtAPI/FileTask.h"
#include "QtAPI/TaskManager.h"
#include "QtAPI/SubWindow.h"

#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <iostream>

MainWindow::MainWindow()
  :QMainWindow()
{

  setAttribute(Qt::WA_DeleteOnClose);
  resize(1200,600);
  m_mdiArea = new QMdiArea(this);
  setCentralWidget(m_mdiArea);

  // AlgorithmExplorer
  m_algorithmExplorer = new AlgorithmExplorer(this);
  m_algorithmExplorer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  this->addDockWidget(Qt::RightDockWidgetArea,m_algorithmExplorer);

  // WorkspaceExplorer
  m_workspaceExplorer = new WorkspaceExplorer(this);
  m_workspaceExplorer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  this->addDockWidget(Qt::RightDockWidgetArea,m_workspaceExplorer);

  // FitBrowser
  m_fitBrowser = new FitBrowser(this);
  m_fitBrowser->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  this->addDockWidget(Qt::LeftDockWidgetArea,m_fitBrowser);

  createMenus();

  QtAPI::WindowManager::createWindowManager(this);

}

MainWindow::~MainWindow()
{
  //std::cerr << "MainWindow destroyed\n";
}

void MainWindow::closeEvent(QCloseEvent*)
{
  Kernel::Framework::instance().unRegisterSingleton("WindowManager",false);
}

QtAPI::SubWindow* MainWindow::newSubWindow(QWidget* widget)
{
  QtAPI::SubWindow* subwnd = new QtAPI::SubWindow(this);
  connect(subwnd,SIGNAL(subWindowBecameActive(QtAPI::SubWindow*)),this,SLOT(subWindowBecameActive(QtAPI::SubWindow*)));
  connect(subwnd,SIGNAL(subWindowClosed(QtAPI::SubWindow*)),this,SLOT(subWindowClosed(QtAPI::SubWindow*)));
  subwnd->setWidget(widget);
  m_mdiArea->addSubWindow(subwnd);
  subwnd->resize(500,400);
  subwnd->show();
  return subwnd;
}

void MainWindow::createMenus()
 {
   // View menu
   ViewTask* viewTask = new ViewTask();
   viewTask->setMainWindow(this);
   QtAPI::TaskManager::instance().add("ViewTask",viewTask);
   customMenuBar(menuBar(),nullptr);
}

void MainWindow::subWindowBecameActive(QtAPI::SubWindow* w)
{
  customMenuBar(menuBar(),w);
}

void MainWindow::subWindowClosed(QtAPI::SubWindow*)
{
  customMenuBar(menuBar(),nullptr);
}
