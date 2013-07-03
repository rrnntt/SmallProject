#ifndef NUMERIC_DVRFUNCTIONBASE_H
#define NUMERIC_DVRFUNCTIONBASE_H

#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomainMD.h"

namespace Numeric
{

class NUMERIC_EXPORT DVRFunctionBase
{
public:
  DVRFunctionBase(FunctionDomainMD *domain);
  ~DVRFunctionBase();
  size_t size() const {return m_domain->size();}
private:
  FunctionDomainMD *m_domain;
};

typedef boost::shared_ptr<DVRFunctionBase> DVRFunctionBase_sptr;

} // Numeric

#endif // NUMERIC_DVRFUNCTIONBASE_H
