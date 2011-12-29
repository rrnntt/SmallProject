#ifndef KERNEL_PROPERTYMANAGER_H_
#define KERNEL_PROPERTYMANAGER_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"
#include "Kernel/Property.h"

#include <map>
#include <set>

namespace Kernel
{

  class KERNEL_EXPORT PropertyManager
  {
    struct Storage
    {
      Property* prop;
    };
  public:
    ///Constructor
    PropertyManager(){}
    ///Virtual Destructor
    virtual ~PropertyManager();
    Property& get(const std::string& name);
    std::set<std::string> getPropertyNames() const;
  protected:
    void declare(const std::string& name, Property* prop);

  protected:
    std::map<std::string,Storage> m_properties;

  };


} // namespace Kernel

#endif //KERNEL_PROPERTYMANAGER_H_
