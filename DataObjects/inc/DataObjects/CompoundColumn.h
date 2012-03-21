#ifndef DATAOBJECTS_COMPOUNDCOLUMN_H
#define DATAOBJECTS_COMPOUNDCOLUMN_H

#include "DataObjects/DllExport.h"

#include <string>

namespace DataObjects
{

class DATAOBJECTS_EXPORT CompoundColumn
{
public:
  CompoundColumn(){}
  virtual ~CompoundColumn(){}
  //virtual size_t componentCount() const = 0;
  //virtual std::string componentAsString(size_t row, const std::string&) const = 0;
  virtual void componentFromString(size_t row, const std::string&, const std::string&) = 0;
};

} // namespace DataObjects

#endif // DATAOBJECTS_COMPOUNDCOLUMN_H
