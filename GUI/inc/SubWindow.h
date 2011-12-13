#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QtGui/QWidget>

class QMenu;

class SubWindow: public QWidget
{
  Q_OBJECT
public:
  SubWindow(QWidget *parent = NULL):QWidget(parent){}
  void setTitle(const QString& title);
  QString title() const {return m_title;}
  virtual QMenu* makeContext() const {return nullptr;}
signals:
  void setMdiTitle(const QString&);
protected:
  QString m_title;
};

#endif // SUBWINDOW_H
