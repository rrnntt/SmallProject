#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtAPI/WindowManager.h"
#include <QtGUI/QMainWindow>

class AlgorithmExplorer;
class WorkspaceExplorer;

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
class MainWindow: public QMainWindow, public QtAPI::WindowManager
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();
  QtAPI::SubWindow* newSubWindow(QWidget* widget);
protected:
  void createMenus();
  void closeEvent(QCloseEvent*);
protected slots:
  void openOSGWindow();
  void openOSGFile();
private:
  QMdiArea* m_mdiArea;
  AlgorithmExplorer* m_algorithmExplorer;
  WorkspaceExplorer* m_workspaceExplorer;
  /*---  Actions  ---*/
  QAction* m_openOSGWindow;
  QAction* m_openOSGFile;
  friend class FileTask;
  friend class ViewTask;
};

#endif /*MAINWINDOW_H*/
