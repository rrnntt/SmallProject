#include "Numeric/ProductFunction.h"

namespace Numeric
{

/**
 * Constructor.
 */
ProductFunction::ProductFunction():
CompositeFunction()
{
}

/**
 * Destructor.
 */
ProductFunction::~ProductFunction()
{
}

/// Function you want to fit to. 
/// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
void ProductFunction::function(const FunctionDomain& domain, FunctionValues& values)const
{
  FunctionValues tmp(domain);
  values.setCalculated(1.0);
  for(size_t iFun = 0; iFun < nFunctions(); ++iFun)
  {
    m_functions[ iFun ]->function(domain,tmp);
    values *= tmp;
  }
}

/// Derivatives of function with respect to active parameters
void ProductFunction::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv( domain, jacobian );
}

} // Numeric
