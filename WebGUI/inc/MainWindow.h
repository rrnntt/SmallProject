#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWebKit/QWebView>
#include <QtGui/QMainWindow>

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
  void linkClicked(const QUrl& url);
private:
  QWebView* m_wView;
};

#endif /*MAINWINDOW_H*/
