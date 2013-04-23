#ifndef NUMERIC_CUSTOMPOLYNOMIAL_H
#define NUMERIC_CUSTOMPOLYNOMIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/Polynomial.h"
#include "Numeric/Chebfun.h"

namespace Numeric
{

class NUMERIC_EXPORT CustomPolynomial: public Polynomial
{
public:
  CustomPolynomial();
  /// Constructor
  CustomPolynomial(int n, const double& startX = -1,const double& endX = 1);
  /// Destructor.
  ~CustomPolynomial();
  /// Returns the function's name
  virtual std::string name()const {return "CustomPolynomial";}
  /// Returns the integral of the weight function
  virtual double weightIntegral() const;

  /* Own public methods */
  double startX() const {return m_fun.startX();}
  double endX() const {return m_fun.endX();}
  /// Set the weight function
  void setWeightFunction(IFunction_const_sptr wgtFun, const double& tol = 1e-16);
protected:
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Return cost shared pointer to the weight function (creates weight function)
  virtual IFunction_const_sptr createWeightFunction() const;
  virtual IFunction_const_sptr createWeightDerivative() const;

  /* Own protected methods */

  chebfun m_fun;

};

} // Numeric

#endif // NUMERIC_CUSTOMPOLYNOMIAL_H
