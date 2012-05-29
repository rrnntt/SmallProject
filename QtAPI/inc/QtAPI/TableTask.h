#ifndef QTAPI_TABLETASK_H
#define QTAPI_TABLETASK_H

#include "QtAPI/DllExport.h"
#include "QtAPI/Task.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QMenu>

class QAction;

namespace API
{
  class TableWorkspace;
}

namespace QtAPI
{

class Table;
class Plot;

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
  void showTable(boost::shared_ptr<API::TableWorkspace> tws);
  void showTable(const QString& wsName);
public slots:
  void showTableDialog();
protected:
  mutable QPointer<Table> m_table;
  QAction* m_showTableDialog;
};

} // namespace QtAPI
#endif // QTAPI_TABLETASK_H
