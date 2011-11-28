#ifndef API_WORKSPACEFACTORY_H
#define API_WORKSPACEFACTORY_H

#include "Kernel/DynamicFactory.h"
#include "API/DllExport.h"
#include "API/Workspace.h"
#include "API/Singleton.h"

namespace API
{

class API_EXPORT WorkspaceFactory: public Kernel::DynamicFactory<Workspace>, protected Singleton
{
  WorkspaceFactory();
public:
  virtual std::string name() const {return "WorkspaceFactory";}
  static WorkspaceFactory& instance();
};

} // API

#define DECLARE_WORKSPACE(type) \
int declare_workspace_##type( (API::WorkspaceFactory::instance().subscribe<type>(#type),0));

#endif // API_WORKSPACEFACTORY_H
