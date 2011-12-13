#include "Table.h"

#include <QtGui/QHeaderView>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QAction>
#include <QtGui/QItemSelectionModel>
#include <QMenu>

#include <iostream>

Table::Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent):
QTableView(parent)
{
  TableModel* model = new TableModel(ws,this);
  setModel(model);
  this->horizontalHeader()->installEventFilter(this);
  this->verticalHeader()->installEventFilter(this);

  // Actions
  m_insertRow = new QAction("Insert row",this);
  connect(m_insertRow,SIGNAL(triggered()),this,SLOT(insertRow()));
}

void Table::contextMenuEvent( QContextMenuEvent* e )
{
  std::cerr << "Context\n";
}

/**
 * Handles context menus of the vertical and horizontal headers.
 */
bool Table::eventFilter(QObject* watched, QEvent* e)
{
  QContextMenuEvent* context_event = dynamic_cast<QContextMenuEvent*>(e);
  if (context_event)
  {
    // context menu for the horizontal header (with column names)
    if (watched == this->horizontalHeader())
    {
      std::cerr << "horizontal\n";
      return true;
    }
    // context menu for the vertical header (with row numbers)
    if (watched == this->verticalHeader())
    {
      std::cerr << "vertical\n";
      QMenu context;
      context.addAction(m_insertRow);
      context.exec(QCursor::pos());
      return true;
    }
  }
  return false;
}

/**
 * Insert a row before currently selected rows.
 */
void Table::insertRow()
{
  QItemSelectionModel* sel = selectionModel();
  if (!sel->hasSelection()) return;
  QModelIndexList	selList = sel->selectedRows();
  if (selList.isEmpty()) return;
  QModelIndex index = selList[0];
  model()->insertRow(index.row());
}

/*-----------------------------------------------------------------*/

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

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex &index,
  const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) 
  {
    m_workspace->getColumn(index.column())->fromString(value.toString().toStdString(),index.row());
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

//bool	TableModel::insertColumns ( int column, int count, const QModelIndex & parent )
//{
//}

bool	TableModel::insertRows ( int row, int count, const QModelIndex & parent )
{
  beginInsertRows(parent,row,row+count-1);
  m_workspace->insertRow(row);
  endInsertRows();
  return true;
}

//bool	TableModel::removeColumns ( int column, int count, const QModelIndex & parent )
//{
//}
//
//bool	TableModel::removeRows ( int row, int count, const QModelIndex & parent )
//{
//}
