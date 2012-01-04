#ifndef API_WORKSPACEPROPERTY_H
#define API_WORKSPACEPROPERTY_H

#include "API/DllExport.h"
#include "API/Workspace.h"
#include "Kernel/Property.h"

#include <string>

namespace API
{

  class API_EXPORT WorkspaceProperty: public Kernel::PropertyType<Workspace_ptr>
  {
  public:
    WorkspaceProperty(Kernel::Property::Direction dir = Kernel::Property::Input);
    virtual Kernel::Property& operator=(const std::string& str);
    virtual operator std::string() const;
    virtual operator Workspace_ptr() const;
    virtual Kernel::Property& operator=(const Workspace_ptr& value);
    void modified();
  protected:
    Workspace_ptr m_value;
    std::string m_name;
    Kernel::Property::Direction m_direction;
  };

} // API

#endif // API_WORKSPACEPROPERTY_H
