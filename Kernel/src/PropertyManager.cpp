#include "Kernel/PropertyManager.h"

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

  Property& PropertyManager::get(const std::string& name)
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

  std::vector<std::string> PropertyManager::getPropertyNames() const
  {
    std::vector<std::string> out;
    for(auto it = m_properties.begin(); it != m_properties.end();++it)
    {
      out.push_back(it->name);
    }
    return out;
  }

} // namespace Kernel
