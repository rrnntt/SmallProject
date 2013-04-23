#ifndef WORKSPACEEXPLORER_H
#define WORKSPACEEXPLORER_H

#include "API/WorkspaceManager.h"

#include <QDockWidget>

class QAction;
class QTreeWidgetItem;

namespace Ui {
  class WorkspaceExplorer;
}

class WorkspaceExplorer : public QDockWidget, public Kernel::NotificationObserver
{
  Q_OBJECT
public:
  WorkspaceExplorer(QWidget *parent);
  ~WorkspaceExplorer();
protected slots:
  void removeWorkspaces();
  void showWorkspaces();
protected:
  bool eventFilter(QObject *obj, QEvent *ev);
private:
  void handleAdd(const API::WorkspaceManager::AddNotification& nt);
  void handleDelete(const API::WorkspaceManager::DeleteNotification& nt);
  void addWorkspace(const std::string& name, API::Workspace_ptr ws);
  void removeWorkspace(API::Workspace_ptr ws);
  Ui::WorkspaceExplorer *ui;

  //QAction *m_removeWorkspaces;
};

#endif // WORKSPACEEXPLORER_H
