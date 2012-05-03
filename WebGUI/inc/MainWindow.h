#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGUI/QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class QAction;

/**
* The main application window
*/
class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();
protected:
  void closeEvent(QCloseEvent*);
protected slots:
private:
  QMdiArea* m_mdiArea;
};

#endif /*MAINWINDOW_H*/
