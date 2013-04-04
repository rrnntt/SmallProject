#include "MainWindow.h"
#include "MyApplication.h"
#include "QtAPI/WindowManager.h"
#include "Kernel/Framework.h"

#include <iostream>

int main( int argc, char** argv )
{
    std::cerr << "OK\n";
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  MyApplication app(argc, argv);
  //Kernel::Framework::instance().openAllLibraries();// it needs a better place to be called from
  MainWindow * wnd = new MainWindow();
  Kernel::Framework::instance().printoutSingletons();
  //wnd->createMenus();
  wnd->show();
  return app.exec();
}
