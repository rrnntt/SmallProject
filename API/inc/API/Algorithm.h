#ifndef API_ALGORITHM_H
#define API_ALGORITHM_H

#include "API/DllExport.h"
#include "Kernel/PropertyManager.h"

#include <boost/shared_ptr.hpp>

#include <string>

namespace Kernel
{
  class Logger;
}

namespace API
{

  class API_EXPORT Algorithm: public Kernel::PropertyManager
  {
  public:
    Algorithm();
    virtual ~Algorithm(){}
    virtual std::string name() const = 0;
    void execute();
  protected:
    virtual void exec() = 0;

    Kernel::Logger& g_log;
  };

  typedef boost::shared_ptr<Algorithm> Algorithm_ptr;

} // API

#endif // API_ALGORITHM_H
