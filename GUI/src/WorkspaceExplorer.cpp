#include "WorkspaceExplorer.h"
#include "ui_WorkspaceExplorer.h"

#include <QTreeWidget>

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
}

WorkspaceExplorer::~WorkspaceExplorer()
{
    delete ui;
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
