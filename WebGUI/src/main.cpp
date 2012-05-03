#include "MainWindow.h"
#include "MyApplication.h"
#include "API/Framework.h"

int main( int argc, char** argv )
{
  API::Framework::instance().parseCommandLine(argc,argv);
  MyApplication app(argc, argv);
  MainWindow * wnd = new MainWindow();
  wnd->show();
  return app.exec();
}
