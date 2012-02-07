#include "MainWindow.h"
#include "QtAPI/WindowManager.h"
#include "API/Framework.h"

#include <QApplication>

int main( int argc, char** argv )
{
  API::Framework::instance().parseCommandLine(argc,argv);
  QApplication app(argc, argv);
  MainWindow * wnd = new MainWindow();
  QtAPI::WindowManager::createWindowManager(wnd);
  wnd->createMenus();
  wnd->show();
  return app.exec();
}
