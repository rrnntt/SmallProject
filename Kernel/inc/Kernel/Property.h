#ifndef KERNEL_PROPERTY_H_
#define KERNEL_PROPERTY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"

#include <string>
#include <iostream>

namespace Kernel
{
  /** 
   * Base class for a property.
   */
  class KERNEL_EXPORT Property
  {
  public:
    ///Virtual Destructor
    virtual ~Property(){}

    virtual Property& operator=(const Property& p);
    virtual Property& operator=(const std::string& str) = 0;
    virtual operator std::string() const = 0;

    template<class P>
    P& as()
    {
      P* p = dynamic_cast<P*>(this);
      if (!p)
      {
        throw std::runtime_error("Incompatible property types");
      }
      return *p;
    }

  };

  template<typename T>
  class PropertyType: public Property
  {
  public:
    virtual operator T() const  = 0;
    virtual Property& operator=(const T& value) = 0;
  };



} // namespace Kernel

KERNEL_EXPORT std::ostream& operator << (std::ostream& ostr, const Kernel::Property& p);

#endif //KERNEL_PROPERTY_H_
