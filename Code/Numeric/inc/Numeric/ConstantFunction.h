#ifndef NUMERIC_CONSTANTFUNCTION_H
#define NUMERIC_CONSTANTFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT ConstantFunction: public IFunction1D, public ParamFunction
{
public:
  ConstantFunction();
  /// overwrite IFunction base class methods
  std::string name()const{return "ConstantFunction";}
  void function1D(double* out, const double* xValues, const size_t nData)const;
  void functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData);
};

} // Numeric

#endif // NUMERIC_CONSTANTFUNCTION_H
