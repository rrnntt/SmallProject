#ifndef NUMERIC_BACKTOBACKEXPONENTIAL_H
#define NUMERIC_BACKTOBACKEXPONENTIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT BackToBackExponential: public IFunction1D, public ParamFunction
{
public:
  /// Constructor
  BackToBackExponential();
  /// Returns the function's name
  virtual std::string name()const {return "BackToBackExponential";}
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
};

} // namespace Numeric
#endif // NUMERIC_BACKTOBACKEXPONENTIAL_H
