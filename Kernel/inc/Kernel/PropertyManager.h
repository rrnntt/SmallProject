#ifndef KERNEL_PROPERTYMANAGER_H_
#define KERNEL_PROPERTYMANAGER_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Kernel/DllExport.h"
#include "Kernel/Property.h"
#include "Kernel/PropertyClassFactory.h"

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
    /// Get reference to the property
    const Property& get(const std::string& name);
    /// Get names of all managed properties
    std::vector<std::string> getPropertyNames() const;
    /// Add a new double property
    void declareDouble(const std::string& name, double defaultValue = 0.0);
    /// Add a new int property
    void declareInt(const std::string& name, int defaultValue = 0);
    /// Add a new bool property
    void declareBool(const std::string& name, bool defaultValue = false);
    /// Add a new string property
    void declareString(const std::string& name, const std::string& defaultValue = "");
    /// Set value of a double property
    void setProperty(const std::string& name, double value);
    /// Set value of a int property
    void setProperty(const std::string& name, int value);
    /// Set value of a bool property
    void setProperty(const std::string& name, bool value);
    /// Set value of a string property
    void setProperty(const std::string& name, const std::string& value);
    /// Set value of a string property
    void setProperty(const std::string& name, const char* value) {setProperty( name, std::string(value) );}

    /* class property */

    /// Add a new class property
    void declareClass(const std::string& name, PropertyClassFactory* valueFactory, Property::Direction dir = Property::InOut);
    /// Set a value of a class property
    void setProperty(const std::string& name, PropertyClass* value);
    /// Set a value of a class property
    void setProperty(const std::string& name, boost::shared_ptr<PropertyClass> value) {setClassProperty( name, value );}
    template<class T>
    void setProperty(const std::string& name, boost::shared_ptr<T> value)
    {
      setProperty( name, boost::static_pointer_cast<PropertyClass>(value) );
    }
    ClassPropertyHelper getClass(const std::string& name) const;

  protected:
    /// Declare a new property
    void declare(const std::string& name, Property* prop);
    /// Return a pointer to a property 
    Property* getPropertyPointer(const std::string& name) const;
    virtual void setClassProperty(const std::string& name, boost::shared_ptr<PropertyClass> value);

    /// Property storage
    std::vector<Storage> m_properties;

  };

} // namespace Kernel

#endif //KERNEL_PROPERTYMANAGER_H_
