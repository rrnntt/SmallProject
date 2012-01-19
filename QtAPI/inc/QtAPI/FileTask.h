#ifndef QtAPI_FILETASK_H
#define QtAPI_FILETASK_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QObject>
#include <QtGui/QMenu>
#include <QtGui/QAction>

namespace QtAPI
{

class QTAPI_EXPORT FileTask: public Task
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

} // QtAPI

#endif /*QtAPI_FILETASK_H*/