#ifndef API_WORKSPACEFACTORY_H
#define API_WORKSPACEFACTORY_H

#include "Kernel/DynamicFactory.h"
#include "API/DllExport.h"
#include "API/Workspace.h"
#include "Kernel/Singleton.h"

namespace API
{

  struct WorkspaceHelper
  {
    WorkspaceHelper(Workspace* workspace):m_workspace(workspace){}
    Workspace* m_workspace;
    template<class W> operator boost::shared_ptr<W>() 
    {
      auto ws = dynamic_cast<W*>(m_workspace);
      if ( !ws )
      {
        throw std::runtime_error("Cannot cast a workspace");
      }
      return boost::shared_ptr<W>(ws);
    }
  };

class API_EXPORT WorkspaceFactory: public Kernel::DynamicFactory<Workspace>, protected Kernel::Singleton
{
  WorkspaceFactory();
public:
  virtual std::string name() const {return "WorkspaceFactory";}
  static WorkspaceFactory& instance();
  WorkspaceHelper createWorkspace(const std::string& type);
};

} // API

#define DECLARE_WORKSPACE(type) \
int declare_workspace_##type( (API::WorkspaceFactory::instance().subscribe<type>(#type),0));

#endif // API_WORKSPACEFACTORY_H
