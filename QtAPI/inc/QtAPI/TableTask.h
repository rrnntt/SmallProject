#ifndef QTAPI_TABLETASK_H
#define QTAPI_TABLETASK_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QMenu>

class QAction;

namespace QtAPI
{

class Table;

class QTAPI_EXPORT TableTask: public Task
{
  Q_OBJECT
public:
  TableTask();
  virtual Type type() const {return Background;}
  virtual QMenu* menu(SubWindow* w = nullptr) const;
  virtual int menuOrder() const {return customMenuOder;}

  QPointer<Table> newTable() const;
  void loadAsciiTable() const;
public slots:
  void showTableDialog();
protected:
  mutable QPointer<Table> m_table;
  QAction* m_showTableDialog;
};

} // namespace QtAPI
#endif // QTAPI_TABLETASK_H
