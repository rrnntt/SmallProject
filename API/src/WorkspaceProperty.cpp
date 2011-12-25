#include "API/WorkspaceProperty.h"
#include "API/WorkspaceManager.h"

#include <stdexcept>

namespace API
{

  /**
    * Assign property form a string.
    * @param str :: Name of the workspace in WorkspaceManager.
    */
  Kernel::Property& WorkspaceProperty::operator=(const std::string& str)
  {
    m_value = WorkspaceManager::instance().retrieve(str);
    return *this;
  }

  /**
    * Return the workspace name.
    */
  WorkspaceProperty::operator std::string() const
  {
    if (m_value)
    {
      return m_value->name();
    }
    return "";
  }

  /**
    * Return shared pointer to the workspace.
    */
  WorkspaceProperty::operator Workspace_ptr() const
  {
    return m_value;
  }

  /**
    * Assign the property from a shared pointer.
    */
  Kernel::Property& WorkspaceProperty::operator=(const Workspace_ptr& value)
  {
    m_value = value;
    return *this;
  }

} // API
