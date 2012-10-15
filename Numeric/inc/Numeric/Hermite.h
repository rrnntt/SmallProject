#ifndef NUMERIC_HERMITE_H
#define NUMERIC_HERMITE_H

#include "Numeric/DllExport.h"
#include "Numeric/Polynomial.h"

namespace Numeric
{

class NUMERIC_EXPORT Hermite: public Polynomial
{
public:
  /// Constructor
  Hermite();
  /// Constructor
  Hermite(int n);
  /// Returns the function's name
  virtual std::string name()const {return "Hermite";}
  /// Returns the integral of the weight function
  virtual double weightIntegral() const;
protected:
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Return cost shared pointer to the weight function (creates weight function)
  virtual IFunction_const_sptr createWeightFunction() const;
  virtual IFunction_const_sptr createWeightDerivative() const;

};

} // Numeric

#endif // NUMERIC_HERMITE_H
