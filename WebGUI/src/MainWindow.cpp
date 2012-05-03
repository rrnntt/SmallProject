#include "MainWindow.h"
//#include "Modeling/OSGWidget.h"

#include "API/Framework.h"

#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtWebKit/QWebView>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <iostream>

MainWindow::MainWindow()
  :QMainWindow()
{
  setAttribute(Qt::WA_DeleteOnClose);
  //setGeometry(900,900,300,200);
  //m_mdiArea = new QMdiArea(this);
  auto wView = new QWebView(this);
  wView->load(QUrl("http://www.google.com/"));
  setCentralWidget(wView);

}

MainWindow::~MainWindow()
{
  //std::cerr << "MainWindow destroyed\n";
}

void MainWindow::closeEvent(QCloseEvent*)
{
}

