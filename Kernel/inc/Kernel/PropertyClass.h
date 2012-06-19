#ifndef KERNEL_PROPERTYCLASS_H
#define KERNEL_PROPERTYCLASS_H

#include "Kernel/DllExport.h"
#include "Kernel/Property.h"

namespace Kernel
{

/**
 * An interface for a class which can be used as a property.
 */
class KERNEL_EXPORT PropertyClass
{
public:
  /// Default constructor
  PropertyClass();
  /// Virtual destructor
  virtual ~PropertyClass();
  /// Return string representation
  virtual std::string asString() const = 0;
  /// Implement if class instance must be stored somewhere. Called from PropertyManager::setProperty()
  /// @param nm :: Name of this property in a PropertyManager
  virtual void store(const std::string& nm) {(void)nm;}
};

} // KERNEL

#endif // KERNEL_PROPERTYCLASS_H
