#ifndef QtAPI_TABLE_H
#define QtAPI_TABLE_H

#include "QtAPI/DllExport.h"
#include "DataObjects/TableWorkspace.h"
#include "API/WorkspaceManager.h"

#include <QtGui/QTableView>
#include <QtCore/QAbstractItemModel>

class QAction;
class QMenu;

namespace QtAPI
{

class QTAPI_EXPORT Table: public QTableView
{
  Q_OBJECT
public:
  Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent = NULL);
signals:
  void showMenu(QMenu*);
protected slots:
  void insertRow();
  void removeRows();
  void insertColumn();
  void removeSelectedColumns();
  void execMenu(QMenu* menu);
protected:
  void	contextMenuEvent( QContextMenuEvent* e );
  bool eventFilter(QObject* watched, QEvent* e);

  QAction* m_insertRow;
  QAction* m_removeRows;
  QAction* m_insertColumn;
  QAction* m_removeSelectedColumns;
  QAction* m_saveAscii;
  QAction* m_showTableDialog;
};

class TableModel: public QAbstractItemModel, public Kernel::NotificationObserver
{
  Q_OBJECT
public:
  TableModel(DataObjects::TableWorkspace_ptr ws,QObject* parent);
  ~TableModel();
  virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const ;
  virtual QVariant	data ( const QModelIndex & index, int role = Qt::DisplayRole ) const ;
  virtual QVariant	headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const ;
  virtual QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
  virtual QModelIndex	parent ( const QModelIndex & index ) const;
  virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
  //virtual bool	insertColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );
  virtual bool	insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  //virtual bool	removeColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );
  virtual bool	removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
  bool insertColumnBefore( int column, const std::string& type, const std::string& name );
  bool removeColumnNumbers(const QList<int>& names);
public slots:
  void saveAscii();
  void showTableDialog();
protected:
  void handleDelete(const API::WorkspaceManager::DeleteNotification& nt);
  void handleModified(const API::WorkspaceManager::ModifiedNotification& nt);

  void resetWorkspace();
  bool isValid() const;

  DataObjects::TableWorkspace_ptr m_workspace;
  mutable int m_rowCount;
  mutable int m_columnCount;
  mutable bool m_invalid;
};

} // QtAPI

#endif /*QtAPI_TABLE_H*/
