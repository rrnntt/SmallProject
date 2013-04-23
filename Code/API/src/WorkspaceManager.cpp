#include "API/WorkspaceManager.h"
#include "Kernel/Framework.h"

#include <iostream>
namespace API
{

namespace
{
  /**
   * Dummy workspace class to represent output workspace property values during
   * when the workspace name is set but the value (pointer) is undefined.
   */
  class DummyWorkspace: public Workspace
  {
  public:
    /// Dummy id
    std::string id()const {return "DummyWorkspace";}
  };
}

WorkspaceManager::WorkspaceManager(const std::string& name):Kernel::DataService<Workspace>(name)
{
}

WorkspaceManager& WorkspaceManager::instance()
{
  Kernel::Singleton* s = Kernel::Framework::instance().getSingleton("WorkspaceManager");
  if (s == nullptr)
  {
    WorkspaceManager *f = new WorkspaceManager("WorkspaceManager");
    Kernel::Framework::instance().registerSingleton("WorkspaceManager",f);
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

/**
 * Implement Kernel::PropertyClassFactory method
 * @param value :: A workspace name which must be in this manager
 */
boost::shared_ptr<Kernel::PropertyClass> WorkspaceManager::createProperty(const std::string& value) const
{
  if ( this->doesExist( value ) )
  {
    return retrieve( value );
  }
  auto dummy = new DummyWorkspace;
  dummy->setName( value );
  return Workspace_ptr( dummy );
}

} // API
