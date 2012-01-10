#include "API/Workspace.h"
#include "API/WorkspaceManager.h"

#include <stdexcept>
#include <algorithm>

namespace API
{

Workspace::Workspace()
{
}

Workspace::~Workspace()
{
}

Workspace::Workspace(const Workspace& c)
{
}

void Workspace::modified()
{
  WorkspaceManager::instance().modified(name());
}

} // API
