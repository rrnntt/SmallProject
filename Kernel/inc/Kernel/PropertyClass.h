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
  PropertyClass();
  virtual ~PropertyClass();
  virtual std::string asString() const = 0;
};

} // KERNEL

#endif // KERNEL_PROPERTYCLASS_H
