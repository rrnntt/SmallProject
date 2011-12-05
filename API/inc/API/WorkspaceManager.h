#ifndef API_WORKSPACEMANAGER_H
#define API_WORKSPACEMANAGER_H

#include "API/DllExport.h"
#include "API/Workspace.h"
#include "API/Singleton.h"
#include "Kernel/DataService.h"

namespace API
{

class API_EXPORT WorkspaceManager: public Kernel::DataService<Workspace>, protected Singleton
{
public:
  virtual std::string name() const {return "WorkspaceManager";}
  static WorkspaceManager& instance();
  void add(const std::string& WorkspaceName,Workspace* ws);
  void add(const std::string& WorkspaceName,Workspace_ptr ws);
  void addOrReplace(const std::string& WorkspaceName,Workspace* ws);
  void addOrReplace(const std::string& WorkspaceName,Workspace_ptr ws);
protected:
  WorkspaceManager(const std::string& name);

};

} // API

#endif /*API_WORKSPACEMANAGER_H*/