#include "Numeric/FunctionComposition.h"
#include "Numeric/FunctionDomain1D.h"

namespace Numeric
{

/**
 * Constructor.
 */
FunctionComposition::FunctionComposition():
CompositeFunction()
{
}

/**
 * Destructor.
 */
FunctionComposition::~FunctionComposition()
{
}

/// Function you want to fit to. 
/// @param domain :: The domain
void FunctionComposition::function(const FunctionDomain& domain, FunctionValues& values)const
{
  FunctionValues tmp(domain);
  values.setCalculated(1.0);
  for(size_t iFun = 0; iFun < nFunctions(); ++iFun)
  {
    FunctionDomain1DView view(tmp.getPointerToCalculated(0),domain.size());
    if ( iFun == 0 )
    {
      m_functions[ iFun ]->function(domain,tmp);
    }
    else
    {
    }
  }
}

/// Derivatives of function with respect to active parameters
void FunctionComposition::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv( domain, jacobian );
}

} // Numeric
