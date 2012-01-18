#ifndef QTAPI_SUBWINDOW_H
#define QTAPI_SUBWINDOW_H

#include "DllExport.h"

#include <QtGui/QWidget>

class QMenu;

namespace QtAPI
{

class QTAPI_EXPORT SubWindow: public QWidget
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

} // QtAPI

#endif // QTAPI_SUBWINDOW_H
