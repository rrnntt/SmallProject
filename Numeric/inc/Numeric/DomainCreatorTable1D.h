#ifndef NUMERIC_DOMAINCREATORTABLE1D_H
#define NUMERIC_DOMAINCREATORTABLE1D_H

#include "Numeric/DllExport.h"
#include "Numeric/DomainCreator.h"

namespace Numeric
{

class NUMERIC_EXPORT DomainCreatorTable1D: public DomainCreator
{
public:
  DomainCreatorTable1D();
  ~DomainCreatorTable1D();
  void createDomain(const IFunction& fun, const Workspace& ws);
  void createDomain(const IFunction& fun, const Workspace& ws, FunctionDomain_sptr& domain, FunctionValues_sptr& values);
};

} // Numeric

#endif // NUMERIC_DOMAINCREATORTABLE1D_H
