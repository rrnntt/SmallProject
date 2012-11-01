#include "Numeric/Polynomial.h"
#include "Numeric/GSLMatrix.h"
#include "Numeric/GSLVector.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"

#include <algorithm>

namespace Numeric
{

/// Default constructor.
Polynomial::Polynomial():IFunction1D(),ParamFunction(),m_n(0),m_c0(1.0)
{
}

/// Constructor.
Polynomial::Polynomial(int n):IFunction1D(),ParamFunction(),m_n(n),m_c0(1.0)
{
}

/**----------------------------------------------------------------
 * Constructor.
 * @param a :: A vector with a-coefficients.
 * @param b :: A vector with b-coefficients.
 * @param c :: A vector with c-coefficients.
 * @param weightFun :: Shared pointer to the weight function.
 * @param weightDeriv :: Shared pointer to the weight function derivative.
 */
Polynomial::Polynomial(
  const std::vector<double>& a, 
  const std::vector<double>& b, 
  const std::vector<double>& c,
  IFunction_const_sptr weightFun,
  IFunction_const_sptr weightDeriv,
  const std::string& aName
  ):
IFunction1D(),
ParamFunction(),
m_n( int(a.size()) ),
m_c0(1.0),
m_a(a),
m_b(b),
m_c(c),
m_weightFunction(weightFun),
m_weightDerivative(weightDeriv),
m_polynomialName(aName)
{
  // check input consistency
  updateABC();
}

/**----------------------------------------------------------------
 * Constructor.
 * @param c0 :: The zeroth order polynomial constant (p(0) == c0)
 * @param a :: A vector with a-coefficients.
 * @param b :: A vector with b-coefficients.
 * @param c :: A vector with c-coefficients.
 * @param weightFun :: Shared pointer to the weight function.
 * @param weightDeriv :: Shared pointer to the weight function derivative.
 */
Polynomial::Polynomial(
  double c0,
  const std::vector<double>& a, 
  const std::vector<double>& b, 
  const std::vector<double>& c,
  IFunction_const_sptr weightFun,
  IFunction_const_sptr weightDeriv,
  const std::string& aName
  ):
IFunction1D(),
ParamFunction(),
m_n( int(a.size()) ),
m_c0(c0),
m_a(a),
m_b(b),
m_c(c),
m_weightFunction(weightFun),
m_weightDerivative(weightDeriv),
m_polynomialName(aName)
{
  // check input consistency
  updateABC();
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
    double p0 = m_c0;
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

/**--------------------------------------------------------------
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

/// Return cost shared pointer to the derivative of the weight function
IFunction_const_sptr Polynomial::weightDerivative() const
{
  if ( !m_weightDerivative )
  {
    m_weightDerivative = this->createWeightDerivative();
  }
  return m_weightDerivative;
}

/// Return cost shared pointer to the derivative of this polynomial
IFunction_const_sptr Polynomial::derivative() const
{
  if ( !m_derivative )
  {
    m_derivative = IFunction_const_sptr( new PolynomialDerivative(*this) );
  }
  return m_derivative;
}

/**------------------------------------------------------------
 * Create a polynomial of a smaller order with the same weight 
 * function as this one.
 * @param m :: Order of the polynomial to create.
 */
boost::shared_ptr<Polynomial> Polynomial::subPolynomial(int m) const
{
  if ( m > m_n ) throw std::invalid_argument("Cannot create a Polynomial of a larger order");
  if ( m_a.empty() ) updateABC();
  std::vector<double> a,b,c;
  a.assign( m_a.begin(), m_a.begin() + m );
  b.assign( m_b.begin(), m_b.begin() + m );
  c.assign( m_c.begin(), m_c.begin() + m );
  auto sp = new Polynomial( a, b, c, weightFunction(), weightDerivative(), name() );
  return Polynomial_sptr( sp );
}

/**------------------------------------------------------------
 * Recalculate (re-fill) m_a, m_b, m_c.
 * The default implementation checks that a,b and c vectors are not empty and if empty
 * throws.
 */
void Polynomial::updateABC() const
{
  if ( m_n == 0 ) throw std::runtime_error("Polynomial wasn't initialized.");
  size_t mn = static_cast<size_t>( m_n );
  if ( m_a.size() != mn || m_b.size() != mn || m_c.size() != mn )
  {
    throw std::runtime_error("Polynomial wasn't initialized.");
  }
}

/**------------------------------------------------------------
 * Returns a shared pointer to the weight function. The default implementation
 * throws.
 */
IFunction_const_sptr Polynomial::createWeightFunction() const
{
  throw std::runtime_error("Polynomial wasn't initialized.");
}

/**------------------------------------------------------------
 * Returns a shared pointer to the weight function derivative. The default implementation
 * throws.
 */
IFunction_const_sptr Polynomial::createWeightDerivative() const
{
  throw std::runtime_error("Polynomial wasn't initialized.");
}

/**------------------------------------------------------------
 */
const std::vector<double>& Polynomial::getA() const
{
  if ( m_a.empty() ) updateABC();
  return m_a;
}

/**------------------------------------------------------------
 */
const std::vector<double>& Polynomial::getB() const
{
  if ( m_b.empty() ) updateABC();
  return m_b;
}

/**------------------------------------------------------------
 */
const std::vector<double>& Polynomial::getC() const
{
  if ( m_c.empty() ) updateABC();
  return m_c;
}

/**------------------------------------------------------------
 */
const double Polynomial::getC0() const
{
  return m_c0;
}

/**------------------------------------------------------------
 */
const std::vector<double>& Polynomial::getRoots() const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  return m_roots;
}

/**------------------------------------------------------------
 */
const std::vector<double>& Polynomial::getWeights() const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  return m_weights;
}

/**------------------------------------------------------------
 * Calculate partial barycentric weights for a sub-set of roots.
 * @param ri :: Sub-set of root indices to be used in the barycentric interpolation.
 * @param w  :: Output vector of partial barycentric weights.
 */
void Polynomial::calcBarycentricWeights(const std::set<size_t>& ri, std::vector<double>& w) const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  const size_t nr = m_roots.size();
  if ( ri.size() > nr )
  {
    throw std::runtime_error("Too many roots for partial barycentric interpolation.");
  }
  if ( *std::max_element(ri.begin(), ri.end()) >= nr )
  {
    throw std::runtime_error("Partial barycentric interpolation contains non-existent root.");
  }
  for(auto i = ri.begin(); i != ri.end(); ++i)
  {
    double d = 1.0;
    for(auto j = ri.begin(); j != ri.end(); ++j)
    {
      if ( i == j ) continue;
      d /= m_roots[*i] - m_roots[*j];
    }
    w.push_back( d );
  }
}

/**------------------------------------------------------------
 * Create a partial quadrature
 * @param ri :: A subset of root indices for which a partial quadrature will be generated.
 * @param w  :: Output vector of partial roots.
 * @param w  :: Output vector of partial weights.
 */
void Polynomial::partialQuadrature(const std::set<size_t>& ri, std::vector<double>& r, std::vector<double>& w) const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  const size_t nr = m_roots.size();
  if ( ri.size() > nr )
  {
    throw std::runtime_error("Too many roots for a partial quadrature.");
  }
  if ( *std::max_element(ri.begin(), ri.end()) >= nr )
  {
    throw std::runtime_error("Partial quadrature contains non-existent roots.");
  }
  // excluded indices
  std::set<size_t> rj;
  for(size_t i = 0; i < nr; ++i) {rj.insert(i);}
  for(auto i = ri.begin(); i != ri.end(); ++i)
  {
    r.push_back( m_roots[*i] );
    w.push_back( m_weights[*i] );
    rj.erase( *i );
  }
  // partial barycentric weights
  std::vector<double> barw;
  calcBarycentricWeights( ri, barw );

  const size_t K = ri.size(); // included roots number
  const size_t N = rj.size(); // excluded roots number
  assert( K + N == nr );

  GSLMatrix S( N, K);
  size_t jj = 0; // incremental excluded index 
  for(auto j = rj.begin(); j != rj.end(); ++j,++jj)
  {
    double s = 0.0;
    size_t ii = 0;
    for(auto i = ri.begin(); i != ri.end(); ++i,++ii)
    {
      const double bw = barw[ii];
      s += bw / (m_roots[*j]-m_roots[*i]);
    }
    ii = 0;
    for(auto i = ri.begin(); i != ri.end(); ++i,++ii)
    {
      const double bw = barw[ii];
      S.set( jj, ii, bw / (m_roots[*j]-m_roots[*i]) / s );
    }
  }

  size_t ii = 0;
  for(auto i = ri.begin(); i != ri.end(); ++i,++ii)
  {
    size_t jj = 0;
    double s = 0.0;
    for(auto j = rj.begin(); j != rj.end(); ++j,++jj)
    {
      s += m_weights[ *j ] * S.get( jj, ii );
    }
    w[ii] += s;
  }
}

/**------------------------------------------------------------
 * Create a partial quadrature-2
 * @param ri :: A subset of root indices for which a partial quadrature will be generated.
 * @param r  :: Output vector of partial roots.
 * @param w  :: Output vector of partial weights.
 */
void Polynomial::partialQuadrature2(const std::set<size_t>& ri, std::vector<double>& r, std::vector<double>& w) const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  const size_t nr = m_roots.size();
  if ( ri.size() > nr )
  {
    throw std::runtime_error("Too many roots for a partial quadrature.");
  }
  if ( *std::max_element(ri.begin(), ri.end()) >= nr )
  {
    throw std::runtime_error("Partial quadrature contains non-existent roots.");
  }
  // partial barycentric weights
  std::vector<double> barw;
  calcBarycentricWeights( ri, barw );
  // size of the partial quadrature
  const size_t mr = ri.size();
  // roots of the created partial quadrature
  for(auto i = ri.begin(); i != ri.end(); ++i)
  {
    r.push_back( m_roots[*i] );
  }
  // find roots and weights of full mr-point quadrature
  auto subPoly = subPolynomial( int(mr) );
  auto sr = subPoly->getRoots();
  auto sw = subPoly->getWeights();

  GSLMatrix S( mr, mr);
  for(auto j = 0; j < mr; ++j)
  {
    double t = 0.0;
    for(auto k = 0; k < mr; ++k)
    {
      const double bw = barw[k];
      t += bw / (sr[j] - r[k]);
    }
    for(auto k = 0; k < mr; ++k)
    {
      const double bw = barw[k];
      S.set( j, k, bw / (sr[j] - r[k]) / t );
    }
  }

  w.resize( mr );
  for(auto k = 0; k < mr; ++k)
  {
    double s = 0.0;
    for(auto j = 0; j < mr; ++j)
    {
      s += sw[ j ] * S.get( j, k );
    }
    w[k] = s;
  }
}

/**------------------------------------------------------------
 * Calculate the polynomial and their derivative values at the integration points.
 * @param funs :: Output function values.
 * @param ders :: Output derivatives.
 */
void Polynomial::calcPolyValues(Polynomial::FuncVector funs, Polynomial::FuncVector ders) const
{
  if ( m_roots.empty() )
  {
    calcRoots();
  }
  const size_t nr = m_roots.size();
  
  if ( funs.size() != nr )
  {
    throw std::runtime_error("FuncVector funs has a wrong size.");
  }

  if ( ders.size() != nr )
  {
    throw std::runtime_error("FuncVector ders has a wrong size.");
  }

  FunctionDomain1DView domain( m_roots );
  FunctionValues wgtValues( domain );
  FunctionValues derValues( domain );
  weightFunction()->function( domain, wgtValues );
  weightDerivative()->function( domain, derValues );

  std::vector<double> wgt( nr );
  std::vector<double> wgt_der( nr );
  for(size_t i = 0; i < nr; ++i)
  {
    if ( funs[i]->size() != nr ) throw std::runtime_error("A vector in FuncVector funs has a wrong size.");
    if ( ders[i]->size() != nr ) throw std::runtime_error("A vector in FuncVector ders has a wrong size.");
    wgt[i] = sqrt( m_weights[i] );
    wgt_der[i] = derValues.getCalculated(i) / wgtValues.getCalculated(i);
  }

  if ( m_a.empty() ) updateABC();
  for(size_t i = 0; i < nr; ++i)
  {
    double p0 = m_c0 * wgt[i];
    double p1 = 0;
    double d0 = 0.0;
    double d1 = 0;
    const double x = m_roots[i];
    (*funs[0])[i] = p0;
    (*ders[0])[i] = d0 + p0 * wgt_der[i];
    if ( m_n > 1 )
    {
      p1 = ( m_c[0] * x - m_a[0] ) * p0;
      d1 = m_c[0] * p0;
      (*funs[1])[i] = p1;
      (*ders[1])[i] = d1 + p1 * wgt_der[i];
      for(size_t j = 1; j < m_n - 1; ++j)
      {
        double p = ( m_c[j] * x - m_a[j] ) * p1 - m_b[j] * p0;
        double d = ( m_c[j] * x - m_a[j] ) * d1 - m_b[j] * d0 + m_c[j] * p1;
        p0 = p1;
        p1 = p;
        d0 = d1;
        d1 = d;
        (*funs[j + 1])[i] = p1;
        (*ders[j + 1])[i] = d1 + p1 * wgt_der[i];
      }
    }
  }
}

/**------------------------------------------------------------
 * Normalize the coefficients so that all "sub-polynomials" have
 * norm == 1.
 */
void Polynomial::normalize()
{
  if ( m_roots.empty() ) calcRoots();
  const size_t nr = m_roots.size();
  if ( m_a.empty() ) updateABC();
  double f0 = 1.0 / (m_c0*m_c0 *sqrt( weightIntegral() ));
  double f1 = 1.0;
  m_c0 /= sqrt( weightIntegral() );
  std::vector<double> p0(nr, m_c0);
  for(size_t j = 0; j < nr; ++j)
  {
    p0[j] *= sqrt( m_weights[j] );
  }
  std::vector<double> p1(nr, 0);
  for(size_t i = 0; i < nr; ++i)
  {
    if ( i == 0 )
    {
      double sum = 0.0;
      m_c[0] /= f0;
      m_a[0] /= f0;
      for(size_t j = 0; j < nr; ++j)
      {
        p1[j] = ( m_c[0] * m_roots[j] - m_a[0] ) * p0[j];
        sum += p1[j] *  p1[j];// * m_weights[j];
      }
      f1 = 1./sqrt( sum );
      m_c[0] *= f1;
      m_a[0] *= f1;
      std::transform(p1.begin(),p1.end(),p1.begin(),std::bind2nd(std::multiplies<double>(),f1));
    }
    else
    {
      double sum = 0.0;
      m_c[i] /= f1;
      m_a[i] /= f1;
      m_b[i] /= f0;
      for(size_t j = 0; j < nr; ++j)
      {
        double p = ( m_c[i] * m_roots[j] - m_a[i] ) * p1[j] - m_b[i] * p0[j];
        sum += p * p;// * m_weights[j];
        p0[j] = p1[j];
        p1[j] = p;
      }
      double f = 1./sqrt( sum );
      m_c[i] *= f;
      m_a[i] *= f;
      m_b[i] *= f;
      std::transform(p1.begin(),p1.end(),p1.begin(),std::bind2nd(std::multiplies<double>(),f));
      f0 = f1;
      f1 = f;
    }
  }
  std::cerr << "OK" << std::endl;
}

//======================================================================
/**
 * Constructor.
 * @param poly :: A polynomial for which a derivaive function is created.
 */
PolynomialDerivative::PolynomialDerivative(const Polynomial& poly):
m_c0(poly.getC0()),
m_a(poly.getA()),
m_b(poly.getB()),
m_c(poly.getC()),
m_n(m_a.size())
{
  
  if ( m_n == 0 || m_b.size() != m_n || m_c.size() != m_n )
  {
    throw std::invalid_argument("Inconsistent Polynomial in PolynomialDerivative.");
  }
}

void PolynomialDerivative::function1D(double* out, const double* xValues, const size_t nData)const
{
  if ( nData == 0 ) return;
  for(size_t i = 0; i < nData; ++i)
  {
    double p0 = m_c0;
    double p1 = 0;
    double d0 = 0.0;
    double d1 = 0;
    const double x = xValues[i];
    if ( m_n == 0 )
    {
      //p1 = p0;
      d1 = 0.0;
    }
    else if ( m_n == 1 )
    {
      //p1 = ( m_c[0] * x - m_a[0] ) * p0;
      d1 = m_c[0] * p0;
    }
    else
    {
      p1 = ( m_c[0] * x - m_a[0] ) * p0;
      d1 = m_c[0] * p0;
      for(size_t j = 1; j < m_n; ++j)
      {
        double p = ( m_c[j] * x - m_a[j] ) * p1 - m_b[j] * p0;
        double d = ( m_c[j] * x - m_a[j] ) * d1 - m_b[j] * d0 + m_c[j] * p1;
        p0 = p1;
        p1 = p;
        d0 = d1;
        d1 = d;
      }
    }
    out[i] = d1;
  }
}

} // Numeric
