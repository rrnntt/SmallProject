#include "Kernel/PropertyManager.h"

#include <stdexcept>
#include <algorithm>

namespace Kernel
{

  PropertyManager::~PropertyManager()
  {
    for(auto it = m_properties.begin(); it != m_properties.end(); ++it)
    {
      if(it->second.prop)
      {
        delete it->second.prop;
      }
    }
    m_properties.clear();
  }

  void PropertyManager::declare(const std::string& name, Property* prop)
  {
    if (m_properties.find(name) != m_properties.end())
    {
      throw std::runtime_error("Property " + name + " already defined");
    }
    Storage s;
    s.prop = prop;
    m_properties[name] = s;
    //auto it = std::find_if(m_properties.begin(),m_properties.end(),[&name](Storage& s){
    //  //if (s.
    //});
  }

  Property& PropertyManager::get(const std::string& name)
  {
    auto it = m_properties.find(name);
    if (it == m_properties.end())
    {
      throw std::runtime_error("Property " + name + " is not defined");
    }
    return *it->second.prop;
  }

} // namespace Kernel
