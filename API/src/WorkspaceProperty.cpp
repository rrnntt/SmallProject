#include "API/WorkspaceProperty.h"
#include "API/WorkspaceManager.h"

#include <stdexcept>

namespace API
{

  WorkspaceProperty::WorkspaceProperty(Kernel::Property::Direction dir):
  m_direction(dir)
  {
  }

  /**
    * Assign property from a string.
    * @param str :: Name of the workspace in WorkspaceManager.
    */
  Kernel::Property& WorkspaceProperty::operator=(const std::string& str)
  {
    if (m_direction != Output)
    {
      m_value = WorkspaceManager::instance().retrieve(str);
    }
    m_name = str;
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
    return m_name;
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
    if (m_direction != Input && !m_name.empty())
    {
      WorkspaceManager::instance().addOrReplace(m_name,value);
    }
    return *this;
  }

} // API
