#include "Table.h"

#include <iostream>

Table::Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent):
QTableView(parent)
{
  TableModel* model = new TableModel(ws,this);
  setModel(model);
}

TableModel::TableModel(DataObjects::TableWorkspace_ptr ws,QObject* parent):
QAbstractItemModel(parent),
m_workspace(ws)
{
}

int	TableModel::columnCount ( const QModelIndex & parent ) const
{
  return static_cast<int>(m_workspace->columnCount());
}

QVariant	TableModel::data ( const QModelIndex & index, int role ) const
{
  if (role == Qt::DisplayRole)
  {
    DataObjects::Column_ptr c = m_workspace->getColumn(index.column());
    return QVariant::fromValue(QString::fromStdString(c->asString(index.row())));
  }
  return QVariant();
}

QVariant TableModel::headerData( int section, Qt::Orientation orientation, int role ) const 
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }
  if (orientation == Qt::Vertical)
  {
    return section;
  }
  else
  {
    return QString::fromStdString(m_workspace->getColumn(section)->name());
  }
}

QModelIndex	TableModel::index ( int row, int column, const QModelIndex & parent ) const
{
  return createIndex(row,column);
}

QModelIndex	TableModel::parent ( const QModelIndex & index ) const
{
  return QModelIndex();
}

int	TableModel::rowCount ( const QModelIndex & parent ) const
{
  return static_cast<int>(m_workspace->rowCount());
}
