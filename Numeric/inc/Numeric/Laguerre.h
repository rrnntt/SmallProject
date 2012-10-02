#ifndef NUMERIC_LAGUERRE_H
#define NUMERIC_LAGUERRE_H

#include "Numeric/DllExport.h"
#include "Numeric/Polynomial.h"

namespace Numeric
{

/**
 * Calculates values of a single scaled generalized Laguerre polynomial h*L^\alpha_n(s*x).
 * n is an attrubute, \alpha, h, and s are parameters
 */
class NUMERIC_EXPORT Laguerre: public Polynomial
{
public:
  /// Constructor
  Laguerre();
  /// Constructor
  Laguerre(double alpha, int n);
  /// Returns the function's name
  virtual std::string name()const {return "Laguerre";}
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);
  /// Find all roots of the polynomial
  virtual void roots( std::vector<double>& r ) const ;
  /// Find all roots of the polynomial
  virtual void myroots( std::vector<double>& r ) const ;
protected:
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Returns the value of the un-scaled generalized Laguerre polynomial
  static double unscaledLaguerre(double alpha, int n, double x);
  /// Create parameters
  void init();
};

} // Numeric

#endif // NUMERIC_LAGUERRE_H
