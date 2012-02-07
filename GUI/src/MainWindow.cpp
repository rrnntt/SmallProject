#include "MainWindow.h"
//#include "Modeling/OSGWidget.h"
#include "ViewTask.h"
#include "AlgorithmExplorer.h"
#include "WorkspaceExplorer.h"

#include "API/Framework.h"

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
  setGeometry(900,900,300,200);
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

  //createMenus();
}

MainWindow::~MainWindow()
{
  std::cerr << "MainWindow destroyed\n";
}

void MainWindow::closeEvent(QCloseEvent*)
{
  API::Framework::instance().unRegisterSingleton("WindowManager",false);
}

QtAPI::SubWindow* MainWindow::newSubWindow(QWidget* widget)
{
  QtAPI::SubWindow* subwnd = new QtAPI::SubWindow(this);
  connect(subwnd,SIGNAL(subWindowBecameActive(QtAPI::SubWindow*)),this,SLOT(subWindowBecameActive(QtAPI::SubWindow*)));
  connect(subwnd,SIGNAL(subWindowClosed(QtAPI::SubWindow*)),this,SLOT(subWindowClosed(QtAPI::SubWindow*)));
  subwnd->setWidget(widget);
  m_mdiArea->addSubWindow(subwnd);
  subwnd->resize(300,200);
  subwnd->show();
  return subwnd;
}

void MainWindow::createMenus()
 {
   //// File menu
   //QtAPI::Task_ptr fileTask = QtAPI::TaskManager::instance().retrieve("FileTask");
   //menuBar()->addMenu(fileTask->menu());

   // View menu
   ViewTask* viewTask = new ViewTask();
   viewTask->setMainWindow(this);
   QtAPI::TaskManager::instance().add("ViewTask",viewTask);
   //menuBar()->addMenu(viewTask->menu());
  this->customMenuBar(menuBar(),nullptr);
}

void MainWindow::subWindowBecameActive(QtAPI::SubWindow* w)
{
  this->customMenuBar(menuBar(),w);
}

void MainWindow::subWindowClosed(QtAPI::SubWindow*)
{
  this->customMenuBar(menuBar(),nullptr);
}
