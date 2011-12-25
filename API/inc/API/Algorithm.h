#ifndef API_ALGORITHM_H
#define API_ALGORITHM_H

#include "API/DllExport.h"

#include <string>

namespace API
{

class API_EXPORT Algorithm
{
public:
  Algorithm(){}
  virtual ~Algorithm(){}
  virtual std::string name() const = 0;
  void execute();
protected:
};

} // API

#endif // API_ALGORITHM_H
