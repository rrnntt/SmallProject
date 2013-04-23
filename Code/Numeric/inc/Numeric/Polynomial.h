#ifndef NUMERIC_POLYNOMIAL_H
#define NUMERIC_POLYNOMIAL_H

#include "Numeric/DllExport.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <set>

namespace Numeric
{

class GSLMatrix;

/**
  @brief A member of an orthogonal polynomial basis. Intentional use: quadratures.

  An orthogonal polynomial basis \f$P_n\f$ satisfies a recurrence relationship:

  \f[
  P_{n+1}=(c_n x - a_n) P_n - b_n P_{n-1}
  \f]

  The concrete classes must implement updateABC() and unscaledWeightIntegral()
  if it returns number other than 1.0.
 */
class NUMERIC_EXPORT Polynomial: public IFunction1D, public ParamFunction
{
public:
  /// Type for function values at integration points
  typedef std::vector< std::vector<double>* > FuncVector;

  /// Default constructor.
  Polynomial();
  /// Constructor.
  Polynomial(int n);
  /// Constructor.
  Polynomial(const std::vector<double>& a, 
    const std::vector<double>& b, 
    const std::vector<double>& c,
    IFunction_const_sptr weightFun,
    IFunction_const_sptr weightDeriv,
    const std::string& aName = "Polynomial"
    );
  /// Constructor.
  Polynomial(double c0,
    const std::vector<double>& a, 
    const std::vector<double>& b, 
    const std::vector<double>& c,
    IFunction_const_sptr weightFun,
    IFunction_const_sptr weightDeriv,
    const std::string& aName = "Polynomial"
    );

  /* Base class virtual methods */
  /// Returns the function's name
  virtual std::string name()const {return m_polynomialName;}
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
  /// Function you want to fit to.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);

  /* Own methods */
  /// Returns the integral of the weight function
  virtual double weightIntegral() const {return 1.0;}
  /// Return cost shared pointer to the weight function
  virtual IFunction_const_sptr weightFunction() const;
  virtual IFunction_const_sptr weightDerivative() const;
  virtual IFunction_const_sptr derivative() const;
  /// Create a polynomial of a smaller order
  virtual boost::shared_ptr<Polynomial> subPolynomial(int n) const;
  /// Get the order of the polynomial
  const int n() const {return m_n;}
  const std::vector<double>& getA() const;
  const std::vector<double>& getB() const;
  const std::vector<double>& getC() const;
  const double getC0() const;
  const std::vector<double>& getRoots() const;
  const std::vector<double>& getWeights() const;

  void partialQuadrature(
    const std::set<size_t>& ri, 
    std::vector<double>& r, 
    std::vector<double>& w,
    GSLMatrix* s = NULL,
    GSLMatrix* t = NULL) const;
  void partialQuadrature2(
    const std::set<size_t>& ri, 
    std::vector<double>& r, 
    std::vector<double>& w,
    GSLMatrix* s = NULL) const;
  void partialQuadrature3(
    const std::vector<double>& r, 
    std::vector<double>& w,
    GSLMatrix* s = NULL,
    GSLMatrix* t = NULL) const;
  void calcBarycentricWeights(const std::set<size_t>& ri, std::vector<double>& w) const;
  
  void calcPolyValues(FuncVector funs, FuncVector ders, bool includeWeights = true) const;
  void normalize();
protected:
  /// Update internal state.
  virtual void updateStateRequired() const;
  /// Recalculate (re-fill) m_a, m_b, m_c
  virtual void updateABC() const;
  /// Return cost shared pointer to the weight function (creates weight function)
  virtual IFunction_const_sptr createWeightFunction() const;
  virtual IFunction_const_sptr createWeightDerivative() const;
  /// Find all roots of the polynomial
  virtual void calcRoots() const;

  /// the n attribute - order of the polynomial
  int m_n;
  /// store the roots
  mutable std::vector<double> m_roots;
  /// store the quadrature weights
  mutable std::vector<double> m_weights;
  /// p0 = m_c0, default == 1
  mutable double m_c0;
  /// p(i) = ( c[i-1] * x - a[i-1] ) * p(i-1) - b[i-1] * p(i-2);
  mutable std::vector<double> m_a;
  mutable std::vector<double> m_b;
  mutable std::vector<double> m_c;
  mutable IFunction_const_sptr m_weightFunction;
  mutable IFunction_const_sptr m_weightDerivative;
  mutable IFunction_const_sptr m_derivative;
  std::string m_polynomialName;
};

typedef boost::shared_ptr<Polynomial> Polynomial_sptr;
typedef boost::shared_ptr<const Polynomial> Polynomial_const_sptr;


/**
 * Derivative of a Polynomial.
 */
class NUMERIC_EXPORT PolynomialDerivative: public IFunction1D, public ParamFunction
{
public:
  /// Constructor.
  PolynomialDerivative(const Polynomial& poly);

  /* Base class virtual methods */
  /// Returns the function's name
  virtual std::string name()const {return "PolynomialDerivative";}
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
protected:
  /// p0 = m_c0, default == 1
  double m_c0;
  /// p(i) = ( c[i-1] * x - a[i-1] ) * p(i-1) - b[i-1] * p(i-2);
  std::vector<double> m_a;
  std::vector<double> m_b;
  std::vector<double> m_c;
  size_t m_n; 
};

} // Numeric

#endif // NUMERIC_POLYNOMIAL_H
