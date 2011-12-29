#include "MainWindow.h"
#include "OSGWidget.h"
#include "FileTask.h"
#include "ViewTask.h"
#include "TaskManager.h"
#include "SubWindow.h"
#include "AlgorithmExplorer.h"

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
  //setGeometry(900,900,300,200);
  m_mdiArea = new QMdiArea(this);
  setCentralWidget(m_mdiArea);

  // AlgorithmExplorer
  m_algorithmExplorer = new AlgorithmExplorer(this);
  m_algorithmExplorer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  this->addDockWidget(Qt::RightDockWidgetArea,m_algorithmExplorer);

  createMenus();
}

MainWindow::~MainWindow()
{
  std::cerr << "MainWindow destroyed\n";
}

QMdiSubWindow* MainWindow::newSubWindow(QWidget* widget)
{
    QMdiSubWindow* subwnd = m_mdiArea->addSubWindow(widget);
    //if (dynamic_cast<SubWindow*>(widget))
    //{
    //  std::cerr << "SubWindow\n";
    //  connect(widget,SIGNAL(setMdiTitle(const QString&)),subwnd,SLOT(setWindowTitle(const QString&)));
    //  subwnd->setWindowTitle(dynamic_cast<SubWindow*>(widget)->title());
    //}
    //else
    //  std::cerr << "Not SubWindow\n";
    subwnd->resize(300,200);
    subwnd->show();
    return subwnd;
}

void MainWindow::createMenus()
 {
   // File menu
   FileTask* fileTask = new FileTask();
   fileTask->setMainWindow(this);
   TaskManager::instance().add("FileTask",fileTask);
   menuBar()->addMenu(fileTask->menu());

   // View menu
   ViewTask* viewTask = new ViewTask();
   viewTask->setMainWindow(this);
   TaskManager::instance().add("ViewTask",viewTask);
   menuBar()->addMenu(viewTask->menu());

   // ----------------- stuff to be delt with later --------------------------------
   QMenu* addMenu = menuBar()->addMenu(tr("&Open"));

   m_openOSGWindow = new QAction("OSG window", this);
   connect(m_openOSGWindow,SIGNAL(triggered()),this,SLOT(openOSGWindow()));
   addMenu->addAction(m_openOSGWindow);

   m_openOSGFile = new QAction("OSG file", this);
   connect(m_openOSGFile,SIGNAL(triggered()),this,SLOT(openOSGFile()));
   addMenu->addAction(m_openOSGFile);
   // -------------------------------------------------------------------------------
}


void MainWindow::openOSGWindow()
{
    OSGWidget* viewWidget = new OSGWidget();
    viewWidget->setGeometry( 100, 100, 800, 600 );
    Model* model = new Model;
    model->load("cow.osg");
    viewWidget->setModel( model );
    newSubWindow(viewWidget);
}

void MainWindow::openOSGFile()
{

  QString fileName = QFileDialog::getOpenFileName(this,
     "Open an OSG file", "", "OSG Files (*.osg *.osgt)");
  if ( !fileName.isEmpty() )
  {
    QFileInfo finfo( fileName );
    QDir::setCurrent( finfo.absoluteDir().absolutePath() );
    OSGWidget* viewWidget = new OSGWidget();
    viewWidget->setGeometry( 100, 100, 800, 600 );
    Model* model = new Model;
    model->load( fileName.toStdString() );
    viewWidget->setModel( model );
    newSubWindow(viewWidget);
  }
}
