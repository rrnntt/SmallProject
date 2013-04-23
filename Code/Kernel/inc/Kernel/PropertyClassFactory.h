#ifndef KERNEL_PROPERTYCLASSFACTORY_H
#define KERNEL_PROPERTYCLASSFACTORY_H

#include "Kernel/DllExport.h"
#include "Kernel/PropertyClass.h"

#include <boost/shared_ptr.hpp>

namespace Kernel
{

class KERNEL_EXPORT PropertyClassFactory
{
public:
  virtual ~PropertyClassFactory(){}
  virtual boost::shared_ptr<PropertyClass> createProperty(const std::string& value) const = 0;
};

} // KERNEL

#endif // KERNEL_PROPERTYCLASSFACTORY_H
