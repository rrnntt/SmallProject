#include "API/WorkspaceFactory.h"
#include "Kernel/Framework.h"

namespace API
{

WorkspaceFactory::WorkspaceFactory()
{
}

WorkspaceFactory& WorkspaceFactory::instance()
{
  Singleton* s = Kernel::Framework::instance().getSingleton("WorkspaceFactory");
  if (s == nullptr)
  {
    WorkspaceFactory *f = new WorkspaceFactory();
    Kernel::Framework::instance().registerSingleton("WorkspaceFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<WorkspaceFactory*>(s);
  }
}

WorkspaceHelper WorkspaceFactory::createWorkspace(const std::string& type)
{
  return WorkspaceHelper( create( type ) );
}

} // API
