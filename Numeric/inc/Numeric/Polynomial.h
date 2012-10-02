#ifndef NUMERIC_POLYNOMIAL_H
#define NUMERIC_POLYNOMIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

namespace Numeric
{

/**
 * A member of an orthogonal polynomial basis. Intentional use:
 * quadratures.
 */
class NUMERIC_EXPORT Polynomial: public IFunction1D, public ParamFunction
{
public:
  /// Default constructor.
  Polynomial():IFunction1D(),ParamFunction(),m_n(0){}
  /// Constructor.
  Polynomial(int n):IFunction1D(),ParamFunction(),m_n(n){}
  /* Base class virtual methods */
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

  /* Own methods */
  /// Find all roots of the polynomial
  virtual void roots( std::vector<double>& r ) const;
  /// Return the quadrature weights
  virtual void weights( std::vector<double>& w ) const;
protected:
  /// Update internal state.
  virtual void updateStateRequired() const;
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const = 0;
  /// the n attribute - order of the polynomial
  int m_n;
  /// store the roots
  mutable std::vector<double> m_roots;
  /// store the quadrature weights
  mutable std::vector<double> m_weights;
  /// p(i) = ( c[i-1] * x - a[i-1] ) * p(i-1) - b[i-1] * p(i-2);
  mutable std::vector<double> m_a;
  mutable std::vector<double> m_b;
  mutable std::vector<double> m_c;
};

} // Numeric

#endif // NUMERIC_POLYNOMIAL_H
