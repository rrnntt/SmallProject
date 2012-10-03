#include "Numeric/LaguerreWeight.h"
#include "Numeric/FunctionFactory.h"

namespace Numeric
{

DECLARE_FUNCTION(LaguerreWeight);

/**
 * Constructor.
 */
LaguerreWeight::LaguerreWeight()
{
  declareParameter("Alpha",0.0,"The alpha parameter of the Laguerre polynomial.");
}

/**
 * Destructor.
 */
LaguerreWeight::~LaguerreWeight()
{
}

/**
 * Function you want to fit.
 */
void LaguerreWeight::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  const double alpha = getParameter("Alpha");
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    out[i] = pow(x, alpha) * exp(-x);
  }
}

/**
 * Derivatives of function with respect to active parameters
 */
void LaguerreWeight::functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData)
{
  if ( nData == 0 ) return;
  const double alpha = getParameter("Alpha");
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    if ( x == 0 )
    {
      // this isn't correct for alpha == 0
      // but we do it anyway to ignore infinity
      out->set( i, 0, 0.0 );
    }
    else
    {
      out->set( i, 0, pow(x, alpha) * exp(-x) * log( x ) );
    }
  }
}

} // Numeric
