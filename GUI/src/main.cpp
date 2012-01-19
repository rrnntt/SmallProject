#include "MainWindow.h"
#include "QtAPI/WindowManager.h"

#include <QApplication>

int main( int argc, char** argv )
{
    QApplication app(argc, argv);
    MainWindow * wnd = new MainWindow();
    QtAPI::WindowManager::createWindowManager(wnd);
    wnd->show();
    return app.exec();
}
