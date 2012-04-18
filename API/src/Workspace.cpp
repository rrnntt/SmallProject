#include "API/Workspace.h"
#include "API/WorkspaceManager.h"

#include <stdexcept>
#include <algorithm>

namespace API
{

void Workspace::modified()
{
  WorkspaceManager::instance().modified(name());
}

} // API
