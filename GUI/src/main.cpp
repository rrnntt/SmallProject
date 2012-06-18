#include "MainWindow.h"
#include "MyApplication.h"
#include "QtAPI/WindowManager.h"
#include "Kernel/Framework.h"

int main( int argc, char** argv )
{
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  MyApplication app(argc, argv);
  MainWindow * wnd = new MainWindow();
  QtAPI::WindowManager::createWindowManager(wnd);
  wnd->createMenus();
  wnd->show();
  return app.exec();
}
