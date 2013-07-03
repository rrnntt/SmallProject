#ifndef NUMERIC_DVRFUNCTION_H
#define NUMERIC_DVRFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/DVRFunctionBase.h"
#include "Numeric/GSLVector.h"

namespace Numeric
{

class NUMERIC_EXPORT DVRFunction
{
public:
  DVRFunction(const DVRFunctionBase_sptr& base);
  DVRFunction(const DVRFunction &fun);
  ~DVRFunction();
 protected:
  /// Function base: domain and weights
  DVRFunctionBase_sptr m_base;
  /// Function values at the base points
  GSLVector m_data;
};

} // Numeric

#endif // NUMERIC_DVRFUNCTION_H
