#include "Numeric/Polynomial.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"

namespace Numeric
{

/// Default constructor.
Polynomial::Polynomial():IFunction1D(),ParamFunction(),m_n(0)
{
  init();
}

/// Constructor.
Polynomial::Polynomial(int n):IFunction1D(),ParamFunction(),m_n(n)
{
  init();
}

/// Create parameters
void Polynomial::init()
{
  declareParameter("Height",1.0,"The function scaling factor.");
  declareParameter("Scaling",1.0,"The argument scaling factor.");
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
  const double height = getParameter("Height");
  const double scaling = getParameter("Scaling");
  for(size_t i = 0; i < nData; ++i)
  {
    double p0 = 1.0;
    double p1 = 0;
    const double x = xValues[i] * scaling;
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
    out[i] = height * p1;
  }
}

/// Derivatives of function with respect to active parameters
void Polynomial::functionDeriv(const FunctionDomain& domain, Jacobian& jacobian)
{
  calNumericalDeriv(domain,jacobian);
}

/**
 * Find all roots of the polynomial
 * @param r :: Vector to store the calculated roots.
 */
void Polynomial::roots( std::vector<double>& r ) const
{
  if ( m_roots.empty() )
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

    const double wgt = weightIntegral();
    for(size_t i = 0; i < m_n; ++i)
    {
      m_roots[i] = x[i];
      double vv = v.get(0, i);
      m_weights[i] = vv * vv * wgt;
    }
    const double scaling = getParameter("Scaling");
    std::transform(m_roots.begin(),m_roots.end(),m_roots.begin(),std::bind2nd(std::multiplies<double>(), 1.0/scaling));
  }

  r.assign(m_roots.begin(), m_roots.end());
}

/**
 * Return the quadrature weights
 * @param w :: Vector to store the weights.
 */
void Polynomial::weights( std::vector<double>& w ) const
{
  if ( m_weights.empty() )
  {
    updateStateRequired();
    std::vector<double> r;
    roots( r );
  }
  w.assign(m_weights.begin(), m_weights.end());
}

/// Returns the integral of the weight function
double Polynomial::weightIntegral() const
{
  const double height = getParameter("Height");
  const double scaling = getParameter("Scaling");
  return unscaledWeightIntegral() * height * scaling;
}

} // Numeric
