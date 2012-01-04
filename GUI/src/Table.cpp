#include "Table.h"
#include "AddTableColumnDialog.h"
#include "API/WorkspaceManager.h"

#include <QtGui/QHeaderView>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QAction>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QFileDialog>
#include <QMenu>

#include <QEvent>
#include <iostream>
#include <vector>
#include <algorithm>

Table::Table(DataObjects::TableWorkspace_ptr ws,QWidget* parent):
QTableView(parent)
{
  TableModel* model = new TableModel(ws,this);
  setModel(model);

  this->horizontalHeader()->installEventFilter(this);
  this->verticalHeader()->installEventFilter(this);
  connect(this,SIGNAL(showMenu(QMenu*)),this,SLOT(execMenu(QMenu*)));

  // Actions
  m_insertRow = new QAction("Insert row",this);
  connect(m_insertRow,SIGNAL(triggered()),this,SLOT(insertRow()));

  m_removeRows = new QAction("Remove rows",this);
  connect(m_removeRows,SIGNAL(triggered()),this,SLOT(removeRows()));

  m_insertColumn = new QAction("Insert column",this);
  connect(m_insertColumn,SIGNAL(triggered()),this,SLOT(insertColumn()));

  m_removeSelectedColumns = new QAction("Remove columns",this);
  connect(m_removeSelectedColumns,SIGNAL(triggered()),this,SLOT(removeSelectedColumns()));

  m_saveAscii = new QAction("Save ASCII",this);
  connect(m_saveAscii,SIGNAL(triggered()),model,SLOT(saveAscii()));

  this->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void Table::contextMenuEvent( QContextMenuEvent* e )
{
  std::cerr << "Context\n";
  QMenu* context = new QMenu(this);
  context->addAction(m_saveAscii);
  emit showMenu(context);
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
      QMenu* context = new QMenu(this);
      context->addAction(m_insertColumn);
      context->addAction(m_removeSelectedColumns);
      emit showMenu(context);
      return true;
    }
    // context menu for the vertical header (with row numbers)
    if (watched == this->verticalHeader())
    {
      QMenu* context = new QMenu(this);
      context->addAction(m_insertRow);
      context->addAction(m_removeRows);
      emit showMenu(context);
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

/**
 * Remove selected rows.
 */
void Table::removeRows()
{
  QItemSelectionModel* sel = selectionModel();
  if (!sel->hasSelection()) return;
  QModelIndexList	selList = sel->selectedRows();
  if (selList.isEmpty()) return;
  std::vector<int> rows;
  foreach(const QModelIndex& index,selList)
  {
    rows.push_back(index.row());
  }
  std::sort(rows.begin(),rows.end(),std::greater<int>());

  auto start = rows.begin();
  for(auto row = start; row != rows.end(); ++row)
  {
    auto row1 = row + 1;
    if (row1 == rows.end())
    {
      std::cerr << "remove " << *row << ' ' << int(row1 - start) << std::endl;
      model()->removeRows(*row,int(row1 - start));
    }
    else if (*row - *row1 != 1)
    {
      model()->removeRows(*row,int(row1 - start));
      if (row1 != rows.end())
      {
        start = row1;
      }
    }
  }
}

void Table::insertColumn()
{
  QItemSelectionModel* sel = selectionModel();
  if (!sel->hasSelection()) return;
  QModelIndexList	selList = sel->selectedColumns();
  if (selList.isEmpty()) return;
  AddTableColumnDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted)
  {
    static_cast<TableModel*>(model())->insertColumnBefore(selList[0].column(),
      dlg.getType(),dlg.getName());
  }
}

void Table::removeSelectedColumns()
{
  QItemSelectionModel* sel = selectionModel();
  if (!sel->hasSelection()) return;
  QModelIndexList	selList = sel->selectedColumns();
  if (selList.isEmpty()) return;
  QList<int> columns;
  foreach(const QModelIndex& index,selList)
  {
    columns << index.column();
  }

  static_cast<TableModel*>(model())->removeColumnNumbers(columns);
}

void Table::execMenu(QMenu* menu)
{
  menu->exec(QCursor::pos());
}

/* --- TableModel --- */

TableModel::TableModel(DataObjects::TableWorkspace_ptr ws,QObject* parent):
QAbstractItemModel(parent),
m_workspace(ws),
m_rowCount(static_cast<int>(ws->rowCount())),
m_columnCount(static_cast<int>(ws->columnCount())),
m_invalid(false)
{
  setHandler(this,&TableModel::handleDelete);
  setHandler(this,&TableModel::handleModified);
  API::WorkspaceManager::instance().notificationCenter.addObserver(this);
}

TableModel::~TableModel()
{
  if (API::WorkspaceManager::instance().doesExist(m_workspace->name()))
  {
    API::WorkspaceManager::instance().remove(m_workspace->name());
  }
  API::WorkspaceManager::instance().notificationCenter.removeObserver(this);
}

void TableModel::resetWorkspace()
{
  m_rowCount = static_cast<int>(m_workspace->rowCount());
  m_columnCount = static_cast<int>(m_workspace->columnCount());
  reset();
}

bool TableModel::isValid() const
{
  m_invalid = (m_workspace->columnCount() != m_columnCount) || 
    (m_workspace->rowCount() != m_rowCount);
  return ! m_invalid;
}

int	TableModel::columnCount ( const QModelIndex & parent ) const
{
  return m_columnCount;
}

QVariant	TableModel::data ( const QModelIndex & index, int role ) const
{
  if (isValid() && (role == Qt::DisplayRole || role == Qt::EditRole) )
  {
    try
    {
      DataObjects::Column_ptr c = m_workspace->getColumn(index.column());
      return QVariant::fromValue(QString::fromStdString(c->asString(index.row())));
    }
    catch(...)
    {
      // do nothing?
    }
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
  else if (isValid())
  {
    return QString::fromStdString(m_workspace->getColumn(section)->name());
  }
  return QVariant();
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
  return m_rowCount;
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
  if (isValid() && index.isValid() && role == Qt::EditRole) 
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
  if (!isValid()) return false;
  beginInsertRows(parent,row,row+count-1);
  m_workspace->insertRow(row);
  endInsertRows();
  return true;
}

//bool	TableModel::removeColumns ( int column, int count, const QModelIndex & parent )
//{
//}

bool	TableModel::removeRows ( int row, int count, const QModelIndex & parent )
{
  if (!isValid()) return false;
  beginRemoveRows(parent,row,row+count-1);
  m_workspace->removeRows(row,count);
  endRemoveRows();
  if ( rowCount() == 0 )
  {
    insertRows(1,1);
  }
  return true;
}

bool TableModel::insertColumnBefore( int column, const std::string& type, const std::string& name )
{
  if (!isValid()) return false;
  if (name.empty() || type.empty()) return false;
  this->beginInsertColumns(QModelIndex(),column,column);
  m_workspace->addColumn(type,name);
  this->endInsertColumns();
  return true;
}


bool TableModel::removeColumnNumbers(const QList<int>& columns)
{
  if (!isValid()) return false;
  std::vector<std::string> names = m_workspace->getColumnNames();
  foreach(int col,columns)
  {
    this->beginRemoveColumns(QModelIndex(),col,col);
    m_workspace->removeColumn(names[col]);
    this->endRemoveColumns();
  }
  if (columnCount() == 0)
  {
    insertColumnBefore(0,"double","X");
  }
  return true;
}

void TableModel::saveAscii()
{
  QString fileName = QFileDialog::getSaveFileName(NULL,"Save Table");
  if ( !fileName.isEmpty() )
  {
    m_workspace->saveAscii(fileName.toStdString());
  }
}

void TableModel::handleDelete(const API::WorkspaceManager::DeleteNotification& nt)
{
}

void TableModel::handleModified(const API::WorkspaceManager::ModifiedNotification& nt)
{
  if (nt.object() == m_workspace)
  {
    resetWorkspace();
  }
}
