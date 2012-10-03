#include "Numeric/JacobiWeight.h"

namespace Numeric
{

/**
 * Constructor.
 */
JacobiWeight::JacobiWeight()
{
  declareParameter("Alpha",0.0,"The alpha parameter of the Jacobi polynomial.");
  declareParameter("Beta",0.0,"The beta parameter of the Jacobi polynomial.");
}

/**
 * Destructor.
 */
JacobiWeight::~JacobiWeight()
{
}

/**
 * Function you want to fit.
 */
void JacobiWeight::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  const double alpha = getParameter("Alpha");
  const double beta = getParameter("Beta");
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    out[i] = pow(1.0 - x, alpha) * pow(1.0 + x, beta);
  }
}

/**
 * Derivatives of function with respect to active parameters
 */
void JacobiWeight::functionDeriv1D(Jacobian* out, const double* xValues, const size_t nData)
{
  if ( nData == 0 ) return;
  const double alpha = getParameter("Alpha");
  const double beta = getParameter("Beta");
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    if ( x == 1.0 || x == -1.0 )
    {
      out->set( i, 0, 0.0 );
      out->set( i, 1, 0.0 );
    }
    else
    {
      const double f = pow(1.0 - x, alpha) * pow(1.0 + x, beta);
      out->set( i, 0, f * log(1.0 - x) );
      out->set( i, 1, f * log(1.0 + x) );
    }
  }
}

} // Numeric