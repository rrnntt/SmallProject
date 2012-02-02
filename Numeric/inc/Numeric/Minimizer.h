#ifndef NUMERIC_MINIMIZER_H
#define NUMERIC_MINIMIZER_H

#include "Numeric/DllExport.h"
#include "Numeric/CostFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT Minimizer
{
public:
  enum Result {Success, Failed};
  Minimizer();
  virtual ~Minimizer(){}
  void setCostFunction(CostFunction_ptr cf);

  virtual Result minimize() = 0;
protected:
  CostFunction_ptr m_costFunction;
};

} // namespace Numeric

#endif // NUMERIC_MINIMIZER_H
