#ifndef KERNEL_PROPERTYMANAGER_H_
#define KERNEL_PROPERTYMANAGER_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"
#include "Kernel/Property.h"

#include <vector>

namespace Kernel
{

  class KERNEL_EXPORT PropertyManager
  {
    struct Storage
    {
      std::string name;
      Property* prop;
    };
  public:
    ///Constructor
    PropertyManager(){}
    ///Virtual Destructor
    virtual ~PropertyManager();
    Property& get(const std::string& name);
    std::vector<std::string> getPropertyNames() const;
  protected:
    void declare(const std::string& name, Property* prop);

  protected:
    std::vector<Storage> m_properties;

  };


} // namespace Kernel

#endif //KERNEL_PROPERTYMANAGER_H_
