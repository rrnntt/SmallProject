#include "WorkspaceExplorer.h"
#include "ui_WorkspaceExplorer.h"

#include "API/WorkspaceManager.h"
#include "QtAPI/TaskManager.h"
#include "QtAPI/TableTask.h"

#include <QTreeWidget>
#include <QMenu>
#include <QAction>

#include <iostream>
#include <stdexcept>

WorkspaceExplorer::WorkspaceExplorer(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::WorkspaceExplorer)
{
  setHandler(this,&WorkspaceExplorer::handleAdd);
  setHandler(this,&WorkspaceExplorer::handleDelete);
  API::WorkspaceManager::instance().notificationCenter.addObserver(this);
  ui->setupUi(this);
  ui->treeWorkspaces->installEventFilter(this);
}

WorkspaceExplorer::~WorkspaceExplorer()
{
  delete ui;
  API::WorkspaceManager::instance().notificationCenter.removeObserver(this);
}

void WorkspaceExplorer::handleAdd(const API::WorkspaceManager::AddNotification& nt)
{
  API::Workspace_ptr ws = nt.object();
  addWorkspace(nt.object_name(), ws);
  ui->treeWorkspaces->update();
}

void WorkspaceExplorer::handleDelete(const API::WorkspaceManager::DeleteNotification& nt)
{
  API::Workspace_ptr ws = nt.object();
  std::cerr<<"Delete notification "<<nt.object_name() << ' ' << ws->name() <<'\n';
  removeWorkspace(ws);
}

void WorkspaceExplorer::addWorkspace(const std::string& name, API::Workspace_ptr ws)
{
  QTreeWidgetItem* treeItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::fromStdString(name)));
  int row = ui->treeWorkspaces->topLevelItemCount();
  ui->treeWorkspaces->insertTopLevelItem(row,treeItem);
}

void WorkspaceExplorer::removeWorkspace(API::Workspace_ptr ws)
{
  QList<QTreeWidgetItem*> list = ui->treeWorkspaces->findItems(QString::fromStdString(ws->name()),Qt::MatchExactly);
  foreach(QTreeWidgetItem* item,list)
  {
    int row = ui->treeWorkspaces->indexOfTopLevelItem(item);
    ui->treeWorkspaces->model()->removeRow(row);
  }
  ui->treeWorkspaces->update();
}

bool WorkspaceExplorer::eventFilter(QObject *obj, QEvent *ev)
{
  QTreeWidget* treeWorkspaces = dynamic_cast<QTreeWidget*>(obj);
  if (treeWorkspaces && ev->type() == QEvent::ContextMenu)
  {
    QList<QTreeWidgetItem *> items = treeWorkspaces->selectedItems();
    if (!items.isEmpty())
    {
      QMenu *context = new QMenu;

      QAction *showWS = new QAction("Show",this);
      connect(showWS,SIGNAL(triggered()),this,SLOT(showWorkspaces()));
      context->addAction(showWS);

      QAction *remove = new QAction("Remove",this);
      connect(remove,SIGNAL(triggered()),this,SLOT(removeWorkspaces()));
      context->addAction(remove);

      context->exec(QCursor::pos());
    }
  }
  return QDockWidget::eventFilter(obj,ev);
}

void WorkspaceExplorer::removeWorkspaces()
{
  QList<QTreeWidgetItem *> items = ui->treeWorkspaces->selectedItems();
  foreach(QTreeWidgetItem* item,items)
  {
    std::string wsName = item->text(0).toStdString();
    try
    {
      API::WorkspaceManager::instance().remove(wsName);
    }
    catch(...)
    {}
  }
}

void WorkspaceExplorer::showWorkspaces()
{
  auto task = QtAPI::TaskManager::instance().getAs<QtAPI::TableTask>("TableTask");
  if (!task) return; // should never happen
  QList<QTreeWidgetItem *> items = ui->treeWorkspaces->selectedItems();
  foreach(QTreeWidgetItem* item,items)
  {
    task->showTable(item->text(0));
  }
}
