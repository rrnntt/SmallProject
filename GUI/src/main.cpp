#include "MainWindow.h"
#include "MyApplication.h"
#include "QtAPI/WindowManager.h"
#include "Kernel/Framework.h"

int main( int argc, char** argv )
{
  Kernel::Framework::instance().parseCommandLine(argc,argv);
  MyApplication app(argc, argv);
  Kernel::Framework::instance().openAllLibraries();// it needs a better place to be called from
  MainWindow * wnd = new MainWindow();
  wnd->show();
  return app.exec();
}
