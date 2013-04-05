#include "Numeric/BackToBackExponential.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/Constants.h"

#include <gsl/gsl_sf_erf.h>

namespace Numeric
{

DECLARE_FUNCTION(BackToBackExponential);

/// Constructor
BackToBackExponential::BackToBackExponential()
{
  declareParameter("I",1.0,"The integrated intensity.");
  declareParameter("a",1.0,"The alpha parameter: rising exponential constant.");
  declareParameter("b",1.0,"The beta parameter: decaying exponential constant.");
  declareParameter("s",1.0,"The sigma parameter: gaussian sigma.");
  declareParameter("x0",0.0,"The peak 'centre' parameter.");
}

/// Function you want to fit to.
void BackToBackExponential::function1D(double* out, const double* xValues, const size_t nData)const
{
  const double& I = getParameter(0);
  const double& a = getParameter(1);
  const double& b = getParameter(2);
  const double& s = getParameter(3);
  const double& x0 = getParameter(4);
  const double s2 = s * s;
  const double s_sqrt_2 = s * Numeric::sqrt_2;

  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i] - x0;
    out[i] = I * ( exp( 0.5*( a*(a*s2 + 2*x) ) + gsl_sf_log_erfc( (a*s2 + x) / s_sqrt_2) )+
                   exp( 0.5*( b*(b*s2 - 2*x) ) + gsl_sf_log_erfc( (b*s2 - x) / s_sqrt_2) ) );
  }
}

} // namespace Numeric
