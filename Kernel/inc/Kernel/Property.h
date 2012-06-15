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
  class PropertyClass;

  /** 
   * Base class for a property.
   */
  class KERNEL_EXPORT Property
  {
  public:

    enum Direction {Input, Output, InOut};
    /// Default constructor
    Property():m_isDefault(true){}
    /// Virtual Destructor
    virtual ~Property(){}

    //virtual Property& operator=(const Property& p);
    virtual Property& operator=(const std::string& str) = 0;
    virtual operator std::string() const = 0;

    /// Is property default?
    bool isDefault() const {return m_isDefault;}

    /// If this is a double property return its value
    operator double() const;
    /// If this is a int property return its value
    operator int() const;
    /// If this is a bool property return its value
    operator bool() const;

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

  protected:

    /// True if the property has its default value
    bool m_isDefault;

    friend class PropertyManager;
  };

  template<typename T>
  class PropertyType: public Property
  {
  public:
    PropertyType():Property(),m_value(){}
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

  /**
   * Property of type PropertyClass
   */
  class ClassProperty: public Property
  {
  public:
    /// Constructor
    ClassProperty(const std::string& type):Property(),m_valueFactoryType(type){}
    /// Set a bare pointer
    void set(PropertyClass* barePointer);
    /// Set a shared pointer
    void set(boost::shared_ptr<PropertyClass> sharedPointer){m_value = sharedPointer;}
    Property& operator=(const std::string& str);
    operator std::string() const;
  protected:
    /// Property value
    boost::shared_ptr<PropertyClass> m_value;

    /// Actual property type
    const std::string m_valueFactoryType;

    friend class PropertyManager;
  };

  struct ClassPropertyHelper
  {
    ClassPropertyHelper(boost::shared_ptr<PropertyClass> value):m_value(value){}
    boost::shared_ptr<PropertyClass> m_value;
    template<class T>
    operator boost::shared_ptr<T>()
    {
      return boost::dynamic_pointer_cast<T>( m_value );
    }
  };

} // namespace Kernel

KERNEL_EXPORT std::ostream& operator << (std::ostream& ostr, const Kernel::Property& p);

#endif //KERNEL_PROPERTY_H_
