#ifndef TABLE_H
#define TABLE_H

#include "DataObjects/TableWorkspace.h"
#include <QtGui/QTableView>
#include <QtCore/QAbstractItemModel>

class Table: public QTableView
{
  Q_OBJECT
public:
  Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent = NULL);

};

class TableModel: public QAbstractItemModel
{
  Q_OBJECT
public:
  TableModel(DataObjects::TableWorkspace_ptr ws,QObject* parent);
  int	columnCount ( const QModelIndex & parent = QModelIndex() ) const ;
  QVariant	data ( const QModelIndex & index, int role = Qt::DisplayRole ) const ;
  QVariant	headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const ;
  QModelIndex	index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
  QModelIndex	parent ( const QModelIndex & index ) const;
  int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
protected:
  DataObjects::TableWorkspace_ptr m_workspace;
};

#endif /*TABLE_H*/