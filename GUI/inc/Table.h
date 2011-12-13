#ifndef TABLE_H
#define TABLE_H

#include "DataObjects/TableWorkspace.h"
#include <QtGui/QTableView>
#include <QtCore/QAbstractItemModel>

class QAction;

class Table: public QTableView
{
  Q_OBJECT
public:
  Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent = NULL);
protected slots:
  void insertRow();
protected:
  void	contextMenuEvent( QContextMenuEvent* e );
  bool eventFilter(QObject* watched, QEvent* e);

  QAction* m_insertRow;
};

class TableModel: public QAbstractItemModel
{
  Q_OBJECT
public:
  TableModel(DataObjects::TableWorkspace_ptr ws,QObject* parent);
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
  //virtual bool	removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
protected:
  DataObjects::TableWorkspace_ptr m_workspace;
};

#endif /*TABLE_H*/