#ifndef NUMERIC_DOMAINCREATOR_H
#define NUMERIC_DOMAINCREATOR_H

#include "Numeric/DllExport.h"

namespace API
{
    class Workspace;
}

namespace Numeric
{

class IFunction;

class NUMERIC_EXPORT DomainCreator
{
public:
  DomainCreator();
  ~DomainCreator();
  /// Create domain and values objects.
  virtual void createDomain(const IFunction& fun, const Workspace& ws, )
};

} // Numeric

#endif // NUMERIC_DOMAINCREATOR_H
