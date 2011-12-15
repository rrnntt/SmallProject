#ifndef FILETASK_H
#define FILETASK_H

#include "Task.h"
#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QAction>

class FileTask: public Task
{
  Q_OBJECT
public:
  FileTask();
  virtual Type type() const{return Background;}
private slots:
  void newTable();
  void loadAsciiTable();
private:
  QAction *m_newTable;
  QAction *m_loadAsciiTable;
};

#endif /*FILETASK_H*/