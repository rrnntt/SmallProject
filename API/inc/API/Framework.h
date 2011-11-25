#ifndef API_FRAMEWORK_H
#define API_FRAMEWORK_H

#include "API/DllExport.h"
#include "API/Singleton.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace API
{

class API_EXPORT Framework
{
public:
  static Framework& instance();
protected:
  Framework();
  boost::ptr_vector<Singleton> m_singletons;
};

} // API

#endif // API_FRAMEWORK_H
