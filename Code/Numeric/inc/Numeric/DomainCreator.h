#ifndef NUMERIC_DOMAINCREATOR_H
#define NUMERIC_DOMAINCREATOR_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"
#include "Numeric/FunctionValues.h"

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
  /// Check if this creator can create a domain for given function and workspace.
  virtual bool canCreateDomain(const IFunction& fun, const API::Workspace& ws) = 0;
  /// Create domain and values objects based on given function and workspace.
  virtual void createDomain(const IFunction& fun, const API::Workspace& ws, FunctionDomain_sptr& domain, FunctionValues_sptr& values) = 0;
};

} // Numeric

#endif // NUMERIC_DOMAINCREATOR_H
