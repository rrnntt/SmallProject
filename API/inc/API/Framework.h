#ifndef API_FRAMEWORK_H
#define API_FRAMEWORK_H

#include "API/DllExport.h"
#include "API/Singleton.h"

//#include <boost/ptr_container/ptr_map.hpp>
#include <map>

namespace API
{

class API_EXPORT Framework
{
public:
  static Framework& instance();
  void registerSingleton(const std::string& name, Singleton* singleton);
  Singleton* getSingleton(const std::string& name);
  ~Framework();
protected:
  Framework();
  //boost::ptr_map<std::string,Singleton> m_singletons;
  std::map<std::string,Singleton*> m_singletons;
};

} // API

#endif // API_FRAMEWORK_H
