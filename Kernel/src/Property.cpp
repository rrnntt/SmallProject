#include "Kernel/Property.h"
#include "Kernel/PropertyClass.h"
#include "Kernel/PropertyClassFactory.h"
#include "Kernel/Framework.h"

#include <stdexcept>

namespace Kernel
{

  //Property& Property::operator=(const Property& p)
  //{
  //  *this = static_cast<std::string>(p);
  //  return *this;
  //}

  /// If this is a double property return its value
  Property::operator double() const
  {
    auto doubleProperty = dynamic_cast<const PropertyType<double>*>( this );
    if ( doubleProperty )
    {
      return *doubleProperty;
    }
    throw std::runtime_error("Property cannot be cast to double");
  }

  /// If this is a int property return its value
  Property::operator int() const
  {
    auto intProperty = dynamic_cast<const PropertyType<int>*>( this );
    if ( intProperty )
    {
      return *intProperty;
    }
    throw std::runtime_error("Property cannot be cast to int");
  }

  /// If this is a bool property return its value
  Property::operator bool() const
  {
    auto boolProperty = dynamic_cast<const PropertyType<bool>*>( this );
    if ( boolProperty )
    {
      return *boolProperty;
    }
    throw std::runtime_error("Property cannot be cast to bool");
  }

  /**
   * Set a bare pointer
   * @param barePointer :: Bare pointer to a value. It will be stored in a shared pointer.
   */
  void ClassProperty::set(PropertyClass* barePointer)
  {
    m_value.reset( barePointer );
  }

  /**
   * Set the property from a string
   */
  Property& ClassProperty::operator=(const std::string& str)
  {
    auto factory = Framework::instance().getSingleton(m_valueFactoryType);
    auto psFactory = dynamic_cast<PropertyClassFactory*>(factory);
    if ( psFactory )
    {
      m_value = psFactory->createProperty( str );
    }

    return *this;
  }

  /**
   * Return the string representation of the property.
   */
  ClassProperty::operator std::string() const
  {
    return m_value ? m_value->asString() : "";
  }


} // namespace Kernel

std::ostream& operator << (std::ostream& ostr, const Kernel::Property& p)
{
  ostr << static_cast<const std::string>(p);
  return ostr;
}
