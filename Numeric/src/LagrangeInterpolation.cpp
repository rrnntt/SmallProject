#include "Numeric/LagrangeInterpolation.h"

namespace Numeric
{

/**
 * Constructor.
 */
LagrangeInterpolation::LagrangeInterpolation(const std::vector<double>& r):
m_r( r )
{
  init();
}

/**
 * Destructor.
 */
LagrangeInterpolation::~LagrangeInterpolation()
{
}

/**
 * Initialise the instance. m_r must be set before calling
 */
void LagrangeInterpolation::init()
{
  const size_t n = size();
  m_D.resize( n, n );
  for(size_t i = 0; i < n; ++i)
  {
    for(size_t j = i; j < n; ++j)
    {
      if ( i == j )
      {
        m_D.set( i, i, 0.0 );
      }
      else
      {
        const double d = m_r[i] - m_r[j];
        m_D.set( i, j, d );
        m_D.set( j, i, -d );
      }
    }
  }
}

/// set a function value
void LagrangeInterpolation::setValue(size_t i, double v)
{
  if ( i >= size() ) throw std::range_error("Index outsize valid range.");
  m_p[i] = v;
}

/// Function you want to fit to.
void LagrangeInterpolation::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  const size_t n = size();
  // distances between current x and a root
  std::vector<double> xr( n );
  // basis functions' values
  std::vector<double> f( n );
  for(size_t i = 0; i < nData; ++i)
  {
    double x = xValues[i];
    double res = 0.0;
    for(size_t j = 0; j < n; ++j)
    {
      xr[j] = x - m_r[j];
      double t = 1.0;
      for(size_t k = 0; k < n; ++k)
      {
        if ( k == j ) continue;
        t *= ( x - m_r[k] ) / m_D.get( j, k );
      }
      res += m_p[j] * t;
    }
    out[i] = res;
  }
}


} // Numeric
