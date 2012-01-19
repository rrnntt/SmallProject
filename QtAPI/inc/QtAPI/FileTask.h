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
  virtual QMenu* menu(SubWindow* w = nullptr) const{return m_menu;}
private slots:
  void newTable();
  void loadAsciiTable();
private:
  QMenu* m_menu;
  QAction *m_newTable;
  QAction *m_loadAsciiTable;
};

} // QtAPI

#endif /*QtAPI_FILETASK_H*/