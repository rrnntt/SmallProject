#include "gtest/gtest.h"
#include "API/Framework.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char **argv) {
  //::testing::InitGoogleTest(&argc, argv);
  API::Framework::instance().parseCommandLine(argc,argv);
  QApplication app(argc, argv);
  //return RUN_ALL_TESTS();
  QMainWindow * wnd = new QMainWindow();
  wnd->show();
  return app.exec();

}
