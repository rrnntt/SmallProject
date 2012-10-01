#include "Numeric/Laguerre.h"
#include "Numeric/FunctionFactory.h"
#include "Numeric/Chebfun.h"

namespace Numeric
{

DECLARE_FUNCTION(Laguerre);

/// Constructor
Laguerre::Laguerre()
{
  declareParameter("Alpha",0.0,"The alpha parameter of the generalized Laguerre polynomial.");
  declareParameter("Height",1.0,"The function scaling factor.");
  declareParameter("Scaling",1.0,"The argument scaling factor.");
}

/// Function you want to fit to.
void Laguerre::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  const double alpha = getParameter("Alpha");
  const double height = getParameter("Height");
  const double scaling = getParameter("Scaling");
  for(size_t i = 0; i < nData; ++i)
  {
    out[i] = height * unscaledLaguerre( alpha, m_n, scaling * xValues[i] );
  }
}

/// Derivatives of function with respect to active parameters
void Laguerre::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv(domain,jacobian);
}

/// Returns the value of the un-scaled generalized Laguerre polynomial
double Laguerre::unscaledLaguerre(double alpha, int n, double x)
{
  double p0 = 1.0;
  if ( n == 0 ) return p0;
  double p1 = -x + alpha + 1.0;
  if ( n == 1 ) return p1;
  double p;
  for(int k = 2; k <= n; ++k)
  {
    p = (2.0 + (alpha - 1.0 - x) / k) * p1 - (1.0 + (alpha - 1.0) / k) * p0;
    p0 = p1;
    p1 = p;
  }
  return p;
}

/// Find all roots of the polynomial
void Laguerre::roots( std::vector<double>& r ) const
{
  if ( m_n == 0 ) 
  {
    r.clear();
    return;
  }

  if ( m_roots.empty() )
  {
    const double alpha = getParameter("Alpha");
    const double scaling = getParameter("Scaling");
    const size_t maxIt = 10;
    const double tol = 3e-14;

    m_roots.resize( m_n );

    // from NR
    for(size_t i = 0; i < m_n; ++i)
    {
      double z;
      // find an approximation for i-th root
      if ( i == 0 )
      {
        z = (1.0 + alpha)*(3.0 + 0.92 * alpha) / (1.0 + 2.4 * m_n + 1.8 * alpha);
      }
      else if ( i == 1 )
      {
        z += (15.0 + 6.25 * alpha) / (1.0 + 0.9 * alpha + 2.5 * m_n);
      }
      else
      {
        const size_t i1 = i - 1;
        z +=  ((1.0 + 2.55 * i1) / (1.9 * i1) + 1.26 * i1 *
          alpha / (1.0 + 3.5 * i1))* (z - m_roots[i1-1]) / (1.0 + 0.3 * alpha);
      }
      // refine the root by newton
      for(size_t it = 0; it < maxIt; ++it)
      {
        double p1 = 1.0;
        double p2 = 0.0;
        for(size_t j = 0; j < m_n; ++j)
        {
          const double p3 = p2;
          p2 = p1;
          const size_t j1 = j + 1;
          p1 = ((2.0*j1 - 1.0 + alpha - z)*p2 - (j1 - 1.0 + alpha)*p3)/j1;
        }
        const double pp = (m_n*p1-(m_n+alpha)*p2)/z;
        const double z1 = z;
        z = z1 - p1/pp;
        if ( fabs(z - z1) <= tol ) break;
        if ( it + 1 == maxIt )
        {
          throw std::runtime_error("Laguerre root finding algorithm didn't converge.");
        }
      }
      m_roots[i] = z;
    } // for i
  }

  r.assign(m_roots.begin(), m_roots.end());
}

} // Numeric
