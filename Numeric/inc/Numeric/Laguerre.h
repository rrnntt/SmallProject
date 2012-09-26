#ifndef NUMERIC_LAGUERRE_H
#define NUMERIC_LAGUERRE_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

/**
 * Calculates values of a single scaled generalized Laguerre polynomial h*L^\alpha_n(s*x).
 * n is an attrubute, \alpha, h, and s are parameters
 */
class NUMERIC_EXPORT Laguerre: public IFunction1D, public ParamFunction
{
public:
  /// Constructor
  Laguerre();
  /// Returns the function's name
  virtual std::string name()const {return "Laguerre";}
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);
  /// Returns the number of attributes associated with the function
  virtual size_t nAttributes()const{return 1;}
  /// Returns a list of attribute names
  virtual std::vector<std::string> getAttributeNames()const;
  /// Return a value of attribute attName
  virtual Attribute getAttribute(const std::string& attName)const;
  /// Set a value to attribute attName
  virtual void setAttribute(const std::string& attName,const Attribute& );
  /// Check if attribute attName exists
  virtual bool hasAttribute(const std::string& attName)const;
protected:
  /// Returns the value of the un-scaled generalized Laguerre polynomial
  static double unscaledLaguerre(double alpha, int n, double x);
  /// the n attribute
  int m_n;
};

} // Numeric

#endif // NUMERIC_LAGUERRE_H
