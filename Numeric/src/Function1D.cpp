#include "Numeric/Function1D.h"

namespace Numeric
{

Function1D::Function1D()
{
}

/// Function you want to fit to. 
/// @param domain :: The buffer for writing the calculated values.
void Function1D::function(FunctionDomain& domain)const
{
  FunctionDomain1D* d1d = dynamic_cast<FunctionDomain1D*>(&domain);
  if (!d1d) throw std::runtime_error("Unexpected domain in fumction " + this->name());
  function1D(*d1d);
}

/// Derivatives of function with respect to parameters
void Function1D::functionDeriv(FunctionDomain& domain, Jacobian& jacobian)
{
  FunctionDomain1D* d1d = dynamic_cast<FunctionDomain1D*>(&domain);
  if (!d1d) throw std::runtime_error("Unexpected domain in fumction " + this->name());
  functionDeriv1D(*d1d,jacobian);
}

} // namespace Numeric
