#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtAPI/WindowManager.h"
#include <QMainWindow>

class AlgorithmExplorer;
class WorkspaceExplorer;
class FitBrowser;

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
  void createMenus();
protected:
  void closeEvent(QCloseEvent*);
protected slots:
  void subWindowBecameActive(QtAPI::SubWindow*);
  void subWindowClosed(QtAPI::SubWindow*);
private:
  //QtAPI::WindowManager* m_windowManager;
  QMdiArea* m_mdiArea;
  AlgorithmExplorer* m_algorithmExplorer;
  WorkspaceExplorer* m_workspaceExplorer;
  FitBrowser* m_fitBrowser;
  friend class FileTask;
  friend class ViewTask;
};

#endif /*MAINWINDOW_H*/
