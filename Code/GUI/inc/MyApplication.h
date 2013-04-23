#ifndef GUI_MYAPPLICATION_H
#define GUI_MYAPPLICATION_H

#include <QApplication>

class MyApplication: public QApplication
{
public:
  MyApplication(int argc, char** argv);
  ~MyApplication();
protected:
  bool notify( QObject *receiver, QEvent *ev );
};

#endif // GUI_MYAPPLICATION_H
