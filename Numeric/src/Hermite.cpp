#include "Numeric/Hermite.h"
#include "Numeric/Constants.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <gsl/gsl_sf_gamma.h>

namespace Numeric
{


namespace
{
  /**
   * The weight function for the Hermite polynomials: exp(-x^2)
   */
  class HermiteWeight: public IFunction1D, public ParamFunction
  {
  public:
    /// Returns the function's name
    virtual std::string name()const {return "HermiteWeight";}
    /// Function you want to fit.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      if ( nData == 0 ) return;
      for(size_t i = 0; i < nData; ++i)
      {
        const double x = xValues[i];
        out[i] = exp(-x*x);
      }
    }
  };

  /**
   * The weight function for the Hermite polynomials: exp(-x^2)
   */
  class HermiteWeightDerivative: public IFunction1D, public ParamFunction
  {
  public:
    /// Returns the function's name
    virtual std::string name()const {return "HermiteWeightDerivative";}
    /// Function you want to fit.
    virtual void function1D(double* out, const double* xValues, const size_t nData)const
    {
      if ( nData == 0 ) return;
      for(size_t i = 0; i < nData; ++i)
      {
        const double x = xValues[i];
        out[i] = 2.0*x*exp(-x*x);
      }
    }
  };

}
/**
 * Constructor.
 */
Hermite::Hermite():Polynomial()
{
}

/**
 * Constructor.
 */
Hermite::Hermite(int n):Polynomial(n)
{
}

/// Recalculate (re-fill) m_a, m_b, m_c
void Hermite::updateABC() const
{
  m_a.resize(m_n);
  m_b.resize(m_n);
  m_c.resize(m_n);

  for(int i = 0; i < m_n; ++i)
  {
    m_c[i] =  2.0;
    m_a[i] =  0;
    m_b[i] =  2.0 * i;
  }
}

/// Returns the unscaled integral of the weight function
double Hermite::weightIntegral() const
{
  return sqrt( pi ) * exp( gsl_sf_lngamma(m_n+1) + m_n *log(2.0) );
}

/// Return cost shared pointer to the weight function (creates weight function)
IFunction_const_sptr Hermite::createWeightFunction() const
{
  return IFunction_const_sptr( new HermiteWeight );
}

IFunction_const_sptr Hermite::createWeightDerivative() const
{
  return IFunction_const_sptr( new HermiteWeightDerivative );
}


} // Numeric
