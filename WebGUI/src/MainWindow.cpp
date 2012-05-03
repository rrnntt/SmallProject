#include "MainWindow.h"
//#include "Modeling/OSGWidget.h"

#include "API/Framework.h"

#include <QtCore/QUrl>
#include <QtGui/QPushButton>

#include <iostream>

MainWindow::MainWindow()
  :QMainWindow()
{
  setAttribute(Qt::WA_DeleteOnClose);
  //setGeometry(900,900,300,200);
  m_wView = new QWebView(this);
  m_wView->load(QUrl("file:///C:/Users/Homer/Desktop/Work/SmallProject/WebGUI/http/index.html"));
  setCentralWidget(m_wView);

  m_wView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  connect(m_wView,SIGNAL(linkClicked(const QUrl&)),this,SLOT(linkClicked(const QUrl&)));

}

MainWindow::~MainWindow()
{
  //std::cerr << "MainWindow destroyed\n";
}

void MainWindow::closeEvent(QCloseEvent*)
{
}

void MainWindow::linkClicked(const QUrl& url)
{
  QString str = url.toString();
  if ( !str.startsWith("cmd:") )
  {
    m_wView->load(url);
  }
  std::cerr << "Link clicked " + url.toString().toStdString() << std::endl;
}
