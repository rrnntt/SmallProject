//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/IFunction1D.h"
#include "Numeric/Jacobian.h"

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <iostream> 

namespace Numeric
{
  
  Kernel::Logger& IFunction1D::g_log = Kernel::Logger::get("IFunction1D");

void IFunction1D::function(const FunctionDomain& domain,FunctionValues& values)const
{
  const FunctionDomain1D* d1d = dynamic_cast<const FunctionDomain1D*>(&domain);
  if (!d1d)
  {
    throw std::invalid_argument("Unexpected domain in IFunction1D");
  }
  function1D(values.getPointerToCalculated(0),d1d->getPointerAt(0),d1d->size());
}

void IFunction1D::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  const FunctionDomain1D* d1d = dynamic_cast<const FunctionDomain1D*>(&domain);
  if (!d1d)
  {
    throw std::invalid_argument("Unexpected domain in IFunction1D");
  }
  try
  {
    functionDeriv1D(&jacobian,d1d->getPointerAt(0),d1d->size());
  }
  catch(...)
  {
    this->calNumericalDeriv(domain,jacobian);
  }
}

/** Base class implementation of derivative IFunction1D throws error. This is to check if such a function is provided
    by derivative class. In the derived classes this method must return the derivatives of the resuduals function
    (defined in void Fit1D::function(const double*, double*, const double*, const double*, const double*, const int&))
    with respect to the fit parameters. If this method is not reimplemented the derivative free simplex minimization
    algorithm is used.
 */
void IFunction1D::functionDeriv1D(Jacobian*, const double*, const size_t)
{
  throw std::runtime_error("No derivative IFunction1D provided");
}

} // namespace Numeric
