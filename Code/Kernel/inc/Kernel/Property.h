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
  //------------------------------------------------------
  //   Forward declarations
  //------------------------------------------------------
  class PropertyClass;
  class PropertyClassFactory;

  /** 
   * Base class for a property.
   */
  class KERNEL_EXPORT Property
  {
  public:

    enum Direction {Input, Output, InOut};
    /// Default constructor
    Property(Direction dir = InOut):m_isDefault(true),m_direction(dir){}
    /// Virtual Destructor
    virtual ~Property(){}

    //virtual Property& operator=(const Property& p);
    virtual Property& operator=(const std::string& str) = 0;
    virtual operator std::string() const = 0;

    /// Is property default?
    bool isDefault() const {return m_isDefault;}
    /// Get the direction of the property
    Direction direction() const {return m_direction;}

    /// If this is a double property return its value
    operator double() const;
    /// If this is a int property return its value
    operator int() const;
    /// If this is a bool property return its value
    operator bool() const;

  protected:

    /// True if the property has its default value
    bool m_isDefault;
    /// Property's direction
    Direction m_direction;

    friend class PropertyManager;
  };

  /**
   * A template for common properties of simple types: double, int, bool and string.
   */
  template<typename T>
  class PropertyType: public Property
  {
  public:
    PropertyType(Direction dir = InOut):Property(dir),m_value(){}
    PropertyType(const T& value, Direction dir = InOut) : Property(dir), m_value(value) {}
    virtual operator T() const  = 0;
    virtual Property& operator=(const T& value) = 0;
  protected:
    T m_value;
  };

  /**
   * Property of type PropertyClass
   */
  class ClassProperty: public Property
  {
  public:
    /// Constructor
    ClassProperty(PropertyClassFactory* valueFactory, Direction dir = InOut):Property(dir),m_valueFactory(valueFactory){}
    /// Set a bare pointer
    void set(PropertyClass* barePointer);
    /// Set a shared pointer
    void set(boost::shared_ptr<PropertyClass> sharedPointer){m_value = sharedPointer;}
    Property& operator=(const std::string& str);
    operator std::string() const;
  protected:
    /// Property value
    boost::shared_ptr<PropertyClass> m_value;

    /// Pointer to the value factory
    PropertyClassFactory* m_valueFactory;

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
