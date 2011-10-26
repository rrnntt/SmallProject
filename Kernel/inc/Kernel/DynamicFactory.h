#ifndef KERNEL_DYNAMICFACTORY_H
#define KERNEL_DYNAMICFACTORY_H

#include "DllExport.h"
#include "Creator.h"

#include <map>
#include <vector>
#include <string>
#include <stdexcept>

namespace Kernel
{

  /**
    * Generec dynamic factory class. Concrete factories will inherit from it.
    */
template<class Base>
class DynamicFactory
{
public:
  DynamicFactory(){}
  virtual ~DynamicFactory()
  {
    for(typename MapType::iterator it=m_map.begin();it!=m_map.end();++it)
    {
      delete it->second;
    }
  }
  /// A typedef for the instantiator
  typedef AbstractCreator<Base> AbstractFactory;

  template<class C>
  void subscribe(const std::string& tname)
  {
    typename MapType::iterator it = m_map.find(tname);
    if (it != m_map.end())
    {
      throw std::runtime_error("Factory type "+tname+" is already defined.");
    }
    m_map[tname] = new Creator<C,Base>();
  }

  /// Create an instance of Base
  Base* create(const std::string& tname)
  {
    typename MapType::iterator it = m_map.find(tname);
    if (it == m_map.end())
    {
      throw std::runtime_error("Factory type "+tname+" is undefined.");
    }
    return m_map[tname]->create();
  }

  /// Check if type has been declared
  bool exists(const std::string& tname)
  {
    return m_map.find(tname) != m_map.end();
  }

  /// Return registered names in a vetor
  std::vector<std::string> keys()const
  {
    std::vector<std::string> res;
    for(typename MapType::const_iterator it=m_map.begin();it!=m_map.end();++it)
    {
      res.push_back(it->first);
    }
    return res;
  }

private:
  /// Copy is not allowed
  DynamicFactory(DynamicFactory&);
  typedef std::map<std::string,AbstractFactory*> MapType;
  /// Holds object creators
  MapType m_map;
};

} // Kernel

#endif // KERNEL_DYNAMICFACTORY_H
