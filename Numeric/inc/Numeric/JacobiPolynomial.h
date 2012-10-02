#ifndef NUMERIC_JACOBIPOLYNOMIAL_H
#define NUMERIC_JACOBIPOLYNOMIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/Polynomial.h"

namespace Numeric
{

class NUMERIC_EXPORT JacobiPolynomial: public Polynomial
{
public:
  /// Constructor
  JacobiPolynomial();
  /// Constructor
  JacobiPolynomial(double alpha, double beta, int n);
  /// Returns the function's name
  virtual std::string name()const {return "JacobiPolynomial";}
protected:
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Returns the unscaled integral of the weight function
  virtual double unscaledWeightIntegral() const;
  /// Create parameters
  void init();
};

} // Numeric

#endif // NUMERIC_JACOBIPOLYNOMIAL_H
