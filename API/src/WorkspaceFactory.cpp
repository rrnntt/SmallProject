#include "API/WorkspaceFactory.h"
#include "API/Framework.h"

namespace API
{

WorkspaceFactory::WorkspaceFactory()
{
}

WorkspaceFactory& WorkspaceFactory::instance()
{
  Singleton* s = Framework::instance().getSingleton("WorkspaceFactory");
  if (s == nullptr)
  {
    WorkspaceFactory *f = new WorkspaceFactory();
    Framework::instance().registerSingleton("WorkspaceFactory",f);
    return *f;
  }
  else
  {
    return *static_cast<WorkspaceFactory*>(s);
  }
}

} // API
