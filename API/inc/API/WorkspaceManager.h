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
  template<class WS>
  std::vector<boost::shared_ptr<WS> > getAllOf() const;

protected:
  WorkspaceManager(const std::string& name);

};

template<class WS>
std::vector<boost::shared_ptr<WS> > WorkspaceManager::getAllOf() const
{
  std::vector<boost::shared_ptr<WS> > out;
  std::set<std::string> names = getObjectNames();
  for(auto it = names.begin(); it != names.end(); ++it)
  {
    boost::shared_ptr<WS> ws = boost::dynamic_pointer_cast<WS>(retrieve(*it));
    if (ws)
    {
      out.push_back(ws);
    }
  }
  return out;
}

} // API

#endif /*API_WORKSPACEMANAGER_H*/