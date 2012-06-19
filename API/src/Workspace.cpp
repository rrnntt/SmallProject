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

/**
 * Store this workspace in WorkspaceManager
 */
void Workspace::store(const std::string& nm)
{
  //WorkspaceManager::instance().addOrReplace();
}

} // API
