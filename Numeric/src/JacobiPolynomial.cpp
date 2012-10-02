#include "Numeric/JacobiPolynomial.h"

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
}

/// Returns the unscaled integral of the weight function
double JacobiPolynomial::unscaledWeightIntegral() const
{
  return 0.0;
}

} // Numeric
