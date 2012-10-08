#include "Numeric/Polynomial.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"

namespace Numeric
{

/// Default constructor.
Polynomial::Polynomial():IFunction1D(),ParamFunction(),m_n(0)
{
}

/// Constructor.
Polynomial::Polynomial(int n):IFunction1D(),ParamFunction(),m_n(n)
{
}

/// Return a value of attribute attName
Polynomial::Attribute Polynomial::getAttribute(const std::string& attName)const
{
  if (attName == "n")
  {
    return Attribute(m_n);
  }
  throw std::runtime_error("Attribute " + attName + " is undefined");
}

/**
 * Set a value to attribute attName
 * @param attName :: Attribute name, must be "Formula"
 * @param att :: Attribute value, must be a math expression, "x" is the function's argument
 */
void Polynomial::setAttribute(const std::string& attName,const Polynomial::Attribute& att)
{
  if (attName == "n")
  {
    m_n = att.asInt();
    updateStateRequired();
  }
  else
  {
    throw std::runtime_error("Attribute " + attName + " is undefined");
  }
}

/// Returns a list of attribute names
std::vector<std::string> Polynomial::getAttributeNames()const
{
  std::vector<std::string> out;
  out.push_back("n");
  return out;
}

/// Check if attribute attName exists
bool Polynomial::hasAttribute(const std::string& attName)const 
{
  return attName == "n";
}

/**
 * Update internal state.
 */
void Polynomial::updateStateRequired() const
{
  if ( !m_roots.empty() )
  {
    m_roots.clear();
    m_weights.clear();
    m_a.clear();
    m_b.clear();
    m_c.clear();
  }
}

/// Function you want to fit to.
void Polynomial::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  if ( m_a.empty() ) updateABC();
  for(size_t i = 0; i < nData; ++i)
  {
    double p0 = 1.0;
    double p1 = 0;
    const double x = xValues[i];
    if ( m_n == 0 )
    {
      p1 = p0;
    }
    else if ( m_n == 1 )
    {
      p1 = ( m_c[0] * x - m_a[0] ) * p0;
    }
    else
    {
      p1 = ( m_c[0] * x - m_a[0] ) * p0;
      for(size_t j = 1; j < m_n; ++j)
      {
        double p = ( m_c[j] * x - m_a[j] ) * p1 - m_b[j] * p0;
        p0 = p1;
        p1 = p;
      }
    }
    out[i] = p1;
  }
}

/// Derivatives of function with respect to active parameters
void Polynomial::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv(domain,jacobian);
}

/**
 * Find all roots of the polynomial
 */
void Polynomial::calcRoots() const
{
  updateABC();
  GSLMatrix J(m_n,m_n);
  J.zero();
  for(size_t i = 0; i < m_n; ++i)
  {
    J.set(i,i, m_a[i] / m_c[i]);
    if ( i < m_n - 1 )
    {
      const size_t i1 = i + 1;
      const double b = sqrt(m_b[i1] / (m_c[i] * m_c[i1]));
      J.set(i, i1, b);
      J.set(i1, i, b);
    }
  }

  GSLVector x( m_n );
  GSLMatrix v( m_n, m_n );
  J.diag( x, v );

  m_roots.resize( m_n );
  m_weights.resize( m_n );

  std::vector<size_t> indx;
  x.indexSort( indx );

  const double wgt = weightIntegral();
  for(size_t i = 0; i < m_n; ++i)
  {
    size_t j = indx[i];
    m_roots[i] = x[j];
    double vv = v.get(0, j);
    m_weights[i] = vv * vv * wgt;
  }
}

/// Return cost shared pointer to the weight function
IFunction_const_sptr Polynomial::weightFunction() const
{
  if ( !m_weightFunction )
  {
    m_weightFunction = this->createWeightFunction();
  }
  return m_weightFunction;
}

/// Return cost shared pointer to the weight function
IFunction_const_sptr Polynomial::weightDerivative() const
{
  if ( !m_weightDerivative )
  {
    m_weightDerivative = this->createWeightDerivative();
  }
  return m_weightDerivative;
}

const std::vector<double>& Polynomial::getA() const
{
  if ( m_a.empty() ) updateABC();
  return m_a;
}

const std::vector<double>& Polynomial::getB() const
{
  if ( m_b.empty() ) updateABC();
  return m_b;
}

const std::vector<double>& Polynomial::getC() const
{
  if ( m_c.empty() ) updateABC();
  return m_c;
}

const std::vector<double>& Polynomial::getRoots() const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  return m_roots;
}

const std::vector<double>& Polynomial::getWeights() const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  return m_weights;
}

} // Numeric
