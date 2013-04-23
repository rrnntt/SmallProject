#ifndef API_COMPOUNDCOLUMN_H
#define API_COMPOUNDCOLUMN_H

#include "API/DllExport.h"

#include <string>

namespace API
{

class API_EXPORT CompoundColumn
{
public:
  CompoundColumn(){}
  virtual ~CompoundColumn(){}
  //virtual size_t componentCount() const = 0;
  //virtual std::string componentAsString(size_t row, const std::string&) const = 0;
  virtual void componentFromString(size_t row, const std::string&, const std::string&) = 0;
};

} // namespace API

#endif // API_COMPOUNDCOLUMN_H
