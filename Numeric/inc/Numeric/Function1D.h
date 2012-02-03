#ifndef NUMERIC_FUNCTION1D_H
#define NUMERIC_FUNCTION1D_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction.h"
#include "Numeric/FunctionDomain1D.h"

namespace Numeric
{

class NUMERIC_EXPORT Function1D: public virtual IFunction
{
public:
  Function1D();
  ~Function1D(){}

  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values.
  virtual void function(FunctionDomain& domain)const;
  /// Derivatives of function with respect to parameters
  virtual void functionDeriv(FunctionDomain& domain, Jacobian& jacobian);
protected:
  virtual void function1D(FunctionDomain1D& domain)const = 0;
  virtual void functionDeriv1D(FunctionDomain1D& domain, Jacobian& jacobian) = 0;
};

} // namespace Numeric
#endif // NUMERIC_FUNCTION1D_H
