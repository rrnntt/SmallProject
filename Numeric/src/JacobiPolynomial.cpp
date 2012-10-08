#include "Numeric/JacobiPolynomial.h"
#include "Numeric/JacobiWeight.h"

#include <gsl/gsl_sf_gamma.h>

namespace Numeric
{

/**
 * Constructor.
 */
JacobiPolynomial::JacobiPolynomial():Polynomial()
{
  init();
}

/**
 * Constructor.
 */
JacobiPolynomial::JacobiPolynomial(double alpha, double beta, int n):Polynomial(n)
{
  init();
  setParameter("Alpha",alpha);
  setParameter("Beta",beta);
}

/// Create parameters
void JacobiPolynomial::init()
{
  declareParameter("Alpha",0.0,"The alpha parameter of the Jacobi polynomial.");
  declareParameter("Beta",0.0,"The beta parameter of the Jacobi polynomial.");
}

/// Recalculate (re-fill) m_a, m_b, m_c
void JacobiPolynomial::updateABC() const
{
  const double alpha = getParameter("Alpha");
  const double beta = getParameter("Beta");

  m_a.resize(m_n);
  m_b.resize(m_n);
  m_c.resize(m_n);

  for(int i = 0; i < m_n; ++i)
  {
    const double n = double(i + 1);
    const double q = 2.0*n + alpha + beta;
    const double r = 2.0*n*(q - n)*(q - 2.0);
    m_c[i] = q * (q - 1.0) * (q - 2.0) / r;
    m_a[i] = (q - 1.0) * (alpha + beta) * (alpha - beta) / r;
    m_b[i] = i == 0? 0.0 : 2.0*q*(n + alpha - 1.0) * (n + beta - 1.0) / r;
  }
}

/// Returns the unscaled integral of the weight function
double JacobiPolynomial::weightIntegral() const
{
  const double alpha = getParameter("Alpha");
  const double beta = getParameter("Beta");

  double res = exp( gsl_sf_lngamma(alpha+1.0) + gsl_sf_lngamma(beta+1.0) 
    - gsl_sf_lngamma(alpha+beta+1.0) );
  return res * pow(2.0, alpha + beta + 1.0) / (alpha + beta + 1.0);
}

/// Return cost shared pointer to the weight function (creates weight function)
IFunction_const_sptr JacobiPolynomial::createWeightFunction() const
{
  JacobiWeight* fun = new JacobiWeight;
  fun->setParameter("Alpha",getParameter("Alpha"));
  fun->setParameter("Beta",getParameter("Beta"));
  return IFunction_const_sptr( fun );
}

IFunction_const_sptr JacobiPolynomial::createWeightDerivative() const
{
}


} // Numeric
