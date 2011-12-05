#include "API/WorkspaceManager.h"
#include "API/Framework.h"

#include <iostream>
namespace API
{

WorkspaceManager::WorkspaceManager(const std::string& name):Kernel::DataService<Workspace>(name)
{
}

WorkspaceManager& WorkspaceManager::instance()
{
  API::Singleton* s = API::Framework::instance().getSingleton("WorkspaceManager");
  if (s == nullptr)
  {
    WorkspaceManager *f = new WorkspaceManager("WorkspaceManager");
    API::Framework::instance().registerSingleton("WorkspaceManager",f);
    return *f;
  }
  else
  {
    return *static_cast<WorkspaceManager*>(s);
  }
}

void WorkspaceManager::add(const std::string& WorkspaceName,Workspace* ws)
{
  WorkspaceManager::add(WorkspaceName,Workspace_ptr(ws));
}

void WorkspaceManager::add(const std::string& WorkspaceName,Workspace_ptr ws)
{
  Kernel::DataService<Workspace>::add(WorkspaceName,ws);
  ws->setName(WorkspaceName);
}

void WorkspaceManager::addOrReplace(const std::string& WorkspaceName,Workspace* ws)
{
  WorkspaceManager::addOrReplace(WorkspaceName,Workspace_ptr(ws));
}

void WorkspaceManager::addOrReplace(const std::string& WorkspaceName,Workspace_ptr ws)
{
  Kernel::DataService<Workspace>::addOrReplace(WorkspaceName,ws);
  ws->setName(WorkspaceName);
}

} // API
