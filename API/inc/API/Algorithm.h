#ifndef API_ALGORITHM_H
#define API_ALGORITHM_H

#include "API/DllExport.h"
#include "Kernel/PropertyManager.h"

#include <string>

namespace API
{

  class API_EXPORT Algorithm: public Kernel::PropertyManager
  {
  public:
    Algorithm(){}
    virtual ~Algorithm(){}
    virtual std::string name() const = 0;
    void execute();
  protected:
    virtual void exec() = 0;
  };

} // API

#endif // API_ALGORITHM_H
