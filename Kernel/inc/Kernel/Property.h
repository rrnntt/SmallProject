#ifndef KERNEL_PROPERTY_H_
#define KERNEL_PROPERTY_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"

#include <string>
#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>

namespace Kernel
{
  /** 
   * Base class for a property.
   */
  class KERNEL_EXPORT Property
  {
  public:

    enum Direction {Input, Output, InOut};
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

    /// Return underlying value of type T
    template<typename T>
    T to();

  };

  template<typename T>
  class PropertyType: public Property
  {
  public:
    PropertyType():m_value(){}
    PropertyType(const T& value) : m_value(value) {}
    virtual operator T() const  = 0;
    virtual Property& operator=(const T& value) = 0;
  protected:
    T m_value;
  };

  template<typename T>
  T Property::to()
  {
    return static_cast<T>(*dynamic_cast<PropertyType<T>* >(this));
  }

} // namespace Kernel

KERNEL_EXPORT std::ostream& operator << (std::ostream& ostr, const Kernel::Property& p);

#endif //KERNEL_PROPERTY_H_
