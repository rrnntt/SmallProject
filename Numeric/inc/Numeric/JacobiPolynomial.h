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
  /// Returns the integral of the weight function
  virtual double weightIntegral() const;
protected:
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Create parameters
  void init();
  /// Return cost shared pointer to the weight function (creates weight function)
  virtual IFunction_const_sptr createWeightFunction() const;
};

} // Numeric

#endif // NUMERIC_JACOBIPOLYNOMIAL_H
