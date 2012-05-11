#ifndef NUMERIC_JOINTDOMAINFUNCTION_H
#define NUMERIC_JOINTDOMAINFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/CompositeFunction.h"

namespace Numeric
{

/**
 * A spline-like function
 */
class NUMERIC_EXPORT JointDomainFunction: public CompositeFunction
{
public:
  JointDomainFunction();
  ~JointDomainFunction();
  /// Returns the function's name
  virtual std::string name()const {return "JointDomainFunction";}
  /// Writes itself into a string
  //std::string asString(bool fmt = false, size_t level = 0)const;
  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(const FunctionDomain& domain, FunctionValues& values)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);
};

} // NUMERIC

#endif // NUMERIC_JOINTDOMAINFUNCTION_H
