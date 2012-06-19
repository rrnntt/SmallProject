#include "Kernel/PropertyManager.h"
#include "Kernel/CommonProperties.h"
#include "Kernel/PropertyClass.h"

#include <stdexcept>
#include <algorithm>

namespace Kernel
{

  PropertyManager::~PropertyManager()
  {
    for(auto it = m_properties.begin(); it != m_properties.end(); ++it)
    {
      if(it->prop)
      {
        delete it->prop;
      }
    }
    m_properties.clear();
  }

  void PropertyManager::declare(const std::string& name, Property* prop)
  {
    auto it = std::find_if(m_properties.begin(),m_properties.end(),[&name](const Storage& s){
      return name == s.name;
    });
    if (it != m_properties.end())
    {
      throw std::runtime_error("Property " + name + " already defined");
    }
    Storage s;
    s.name = name;
    s.prop = prop;
    m_properties.push_back(s);
  }

  /**
   * Add a new double property
   * @param name :: Property name. Must be unique.
   * @param defaultValue :: Optional default value
   */
  void PropertyManager::declareDouble(const std::string& name, double defaultValue)
  {
    declare( name, new DoubleProperty(defaultValue) );
  }

  /**
   * Add a new int property
   * @param name :: Property name. Must be unique.
   * @param defaultValue :: Optional default value
   */
  void PropertyManager::declareInt(const std::string& name, int defaultValue)
  {
    declare( name, new IntProperty(defaultValue) );
  }

  /**
   * Add a new bool property
   * @param name :: Property name. Must be unique.
   * @param defaultValue :: Optional default value
   */
  void PropertyManager::declareBool(const std::string& name, bool defaultValue)
  {
    declare( name, new BoolProperty(defaultValue) );
  }

  /**
   * Add a new string property
   * @param name :: Property name. Must be unique.
   * @param defaultValue :: Optional default value
   */
  void PropertyManager::declareString(const std::string& name, const std::string& defaultValue)
  {
    declare( name, new StringProperty(defaultValue) );
  }

  /**
   * Set value of a double property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setProperty(const std::string& name, double value)
  {
    auto prop = dynamic_cast<PropertyType<double>*>( getPropertyPointer(name) );
    if ( prop ) 
    {
      *prop = value;
      prop->m_isDefault = false;
    }
    else
    {
      throw std::runtime_error("Property cannot be cast to double");
    }
  }

  /**
   * Set value of a int property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setProperty(const std::string& name, int value)
  {
    auto prop = dynamic_cast<PropertyType<int>*>( getPropertyPointer(name) );
    if ( prop ) 
    {
      *prop = value;
      prop->m_isDefault = false;
    }
    else
    {
      throw std::runtime_error("Property cannot be cast to int");
    }
  }

  /**
   * Set value of a bool property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setProperty(const std::string& name, bool value)
  {
    auto prop = dynamic_cast<PropertyType<bool>*>( getPropertyPointer(name) );
    if ( prop ) 
    {
      *prop = value;
      prop->m_isDefault = false;
    }
    else
    {
      throw std::runtime_error("Property cannot be cast to bool");
    }
  }

  /**
   * Set value of a string property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setProperty(const std::string& name, const std::string& value)
  {
    auto prop = getPropertyPointer(name);
    *prop = value;
     prop->m_isDefault = false;
  }

  /**
   * Set a value of a class property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setProperty(const std::string& name, PropertyClass* value)
  {
    auto prop = dynamic_cast<ClassProperty*>( getPropertyPointer(name) );
    if ( !prop )
    {
      throw std::runtime_error("Property " + name + " is not a class.");
    }
    prop->m_value.reset( value );
    //value->store(); // store a workspace (or something) in a data storage
  }

  /**
   * Set a value of a class property
   * @param name :: Property name
   * @param value :: New property value
   */
  void PropertyManager::setClassProperty(const std::string& name, boost::shared_ptr<PropertyClass> value)
  {
    auto prop = dynamic_cast<ClassProperty*>( getPropertyPointer(name) );
    if ( !prop )
    {
      throw std::runtime_error("Property " + name + " is not a class.");
    }
    prop->m_value = value;
    //value->store(); // store a workspace (or something) in a data storage
  }


  /**
   * Return a pointer to a property 
   * @param name :: Property name
   * @return :: Pointer to the property if it exists or throws
   */
  Property* PropertyManager::getPropertyPointer(const std::string& name) const
  {
    auto it = std::find_if(m_properties.begin(),m_properties.end(),[&name](const Storage& s){
      return name == s.name;
    });
    if (it == m_properties.end())
    {
      throw std::runtime_error("Property " + name + " is not defined");
    }
    return it->prop;
  }

  /**
   * Return a const reference to a property 
   * @param name :: Property name
   * @return :: Reference to the property if it exists or throws
   */
  const Property& PropertyManager::get(const std::string& name)
  {
    auto it = std::find_if(m_properties.begin(),m_properties.end(),[&name](const Storage& s){
      return name == s.name;
    });
    if (it == m_properties.end())
    {
      throw std::runtime_error("Property " + name + " is not defined");
    }
    return *it->prop;
  }

  /**
   * Get names of all managed properties
   */
  std::vector<std::string> PropertyManager::getPropertyNames() const
  {
    std::vector<std::string> out;
    for(auto it = m_properties.begin(); it != m_properties.end();++it)
    {
      out.push_back(it->name);
    }
    return out;
  }

  /**
   * Add a new class property
   * @param name :: Property name
   */
  void PropertyManager::declareClass(const std::string& name, PropertyClassFactory* valueFactory, Property::Direction dir)
  {
    declare( name, new ClassProperty( valueFactory, dir ) );
  }

  ClassPropertyHelper PropertyManager::getClass(const std::string& name) const
  {
    auto prop = dynamic_cast<ClassProperty*>( getPropertyPointer( name ) );
    if ( !prop )
    {
      throw std::runtime_error("Property " + name + " is not a class property");
    }
    return ClassPropertyHelper(prop->m_value);
  }


} // namespace Kernel
