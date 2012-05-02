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
   * Copy constructor
   */
  WorkspaceProperty::WorkspaceProperty(const WorkspaceProperty& value)
  {
    *this = value;
  }

  /**
   * Copy assignment.
   */
  WorkspaceProperty& WorkspaceProperty::operator=(const WorkspaceProperty& value)
  {
    m_direction = value.m_direction;
    m_name = value.m_name;
    m_value = value.m_value;
    return *this;
  }

  /**
    * Assign property from a string.
    * @param str :: Name of the workspace in WorkspaceManager.
    */
  Kernel::Property& WorkspaceProperty::operator=(const std::string& str)
  {
    if (m_direction != Output)
    {
      // Input or InOut properties cannot be re-assigned
      if (m_value)
      {
        throw std::runtime_error("Input workspace property cannot be re-assigned");
      }
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
    // Input properties cannot be re-assigned
    if (m_direction == Input && m_value)
    {
      throw std::runtime_error("Input workspace property cannot be re-assigned");
    }
    m_value = value;
    if (!m_name.empty() && (m_direction == Output || 
      (m_direction == InOut && WorkspaceManager::instance().doesExist(m_name)) ))
    {
      WorkspaceManager::instance().addOrReplace(m_name,value);
    }
    return *this;
  }

  /**
   * Send ModifiedNotification
   */
  void WorkspaceProperty::modified()
  {
    if (!m_name.empty() && WorkspaceManager::instance().doesExist(m_name))
    {
      WorkspaceManager::instance().modified(m_name);
    }
  }

} // API
