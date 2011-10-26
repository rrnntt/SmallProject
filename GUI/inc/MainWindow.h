#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGUI/QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class QAction;

namespace osg
{
  class Camera;
}

/**
* The main application window
*/
class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();
  QMdiSubWindow* newSubWindow(QWidget* widget);
protected:
  void createMenus();
protected slots:
  void openOSGWindow();
  void openOSGFile();
private:
  QMdiArea* m_mdiArea;
  /*---  Actions  ---*/
  QAction* m_openOSGWindow;
  QAction* m_openOSGFile;
};

#endif /*MAINWINDOW_H*/