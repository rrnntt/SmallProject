#ifndef NUMERIC_PRODUCTFUNCTION_H
#define NUMERIC_PRODUCTFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/CompositeFunction.h"

namespace Numeric
{

class NUMERIC_EXPORT ProductFunction: public CompositeFunction
{
public:
  ProductFunction();
  ~ProductFunction();
  /// Returns the function's name
  virtual std::string name()const {return "";}
  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(const FunctionDomain& domain, FunctionValues& values)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);
};

} // Numeric

#endif // NUMERIC_PRODUCTFUNCTION_H
