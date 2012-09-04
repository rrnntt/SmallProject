#include "Numeric/ChebFunction.h"

#include <algorithm>

namespace Numeric
{

ChebFunction::ChebFunction():
m_fun(1,new ScaledChebfun)
{
}

/**
 * Copy constructor
 * @param other :: Other function to copy from
 */
ChebFunction::ChebFunction(const ChebFunction& other, bool copy)
{
  m_fun.resize( other.m_fun.size() );
  for(size_t i = 0; i < m_fun.size(); ++i)
  {
    ScaledChebfun* scf = nullptr;
    if ( copy ) scf = new ScaledChebfun( *other.m_fun[i] );
    else
    {
      scf = new ScaledChebfun;
      scf->setBaseFrom( *other.m_fun[i] );
    }
    m_fun[i] = scf;
  }
}

/**
 * Create and init with a single chebfun
 * @param fun :: A chebfun to copy the data from. 
 */
ChebFunction::ChebFunction(const chebfun& fun):
m_fun(1,new ScaledChebfun(fun))
{
}

/**
 * Create and init with a single empty chebfun
 * @param n :: Number of points in the interval. 
 * @param startX :: The left interval boundary. 
 * @param endX :: The right interval boundary. 
 */
ChebFunction::ChebFunction(size_t n, const double& startX,const double& endX):
m_fun(1,new ScaledChebfun(n,startX,endX))
{
}

/**
 * Destructor - clean up.
 */
ChebFunction::~ChebFunction()
{
  clear();
}

/// clear all data
void ChebFunction::clear()
{
  for(auto f = m_fun.begin(); f != m_fun.end(); ++f)
  {
    delete *f;
  }
  m_fun.clear();
}

/**
 * Add an empty ScaledChebfun to the right on the x-axis. The new scaled chebfun will have
 * the interval starting an the end of the previous chebfun and ending at endX.
 * @param n :: Number of points in the interval of the new function.  
 * @param endx :: The right interval boundary.
 */
void ChebFunction::appendRight(size_t n, const double& endx)
{
  if ( endX() == inf ) throw std::runtime_error("Cannot append a chebfun at infinity");
  if ( endx <= endX() ) throw std::runtime_error("New interval is negative or zero");
  m_fun.push_back( new ScaledChebfun(n, endX(), endx) );
}

ChebFunction& ChebFunction::operator=(const ChebFunction& cws)
{
  clear();
  m_fun.resize( cws.m_fun.size() );
  for(size_t i = 0; i < m_fun.size(); ++i)
  {
    m_fun[i] = new ScaledChebfun( *cws.m_fun[i] );
  }
  return *this;
}

/// Function you want to fit to.
void ChebFunction::function1D(double* out, const double* xValues, const size_t nData)const
{
  const double start = startX();
  const double end   = endX();

  size_t fi = 0;
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    if ( x < start ) continue;
    if ( x > end ) break;
    auto f = m_fun[fi];
    while( x > f->endX() )
    {
      ++fi;
      if ( fi == m_fun.size() )
      {
        break;
      }
      f = m_fun[fi];
    }
    out[i] = f->value( x );
  }
}

/**
 * Fill a vector with x values.
 * @param x :: Vector to get the values.
 */
void ChebFunction::fillXValues(std::vector<double>& x) const
{
  size_t npts = 0;
  std::for_each(m_fun.begin(),m_fun.end(),[&npts](const ScaledChebfun* fun){
    npts += fun->n();
  });
  ++npts;
  x.resize( npts );

  size_t i = 0; // running index
  for(auto f = m_fun.begin(); f != m_fun.end(); ++f)
  {
    std::vector<double> xf;
    (**f).fillXValues( xf );
    // there are no gaps between chebfuns
    std::copy(xf.begin(), xf.end() - 1, x.begin() + i);
    i += xf.size() - 1;
  }
  x.back() = this->endX();
}

/// Creates a domain for the region on which the workspace is defined.
FunctionDomain1D_sptr ChebFunction::createDomainFromXPoints() const
{
  std::vector<double> x;
  fillXValues( x );
  auto domain = new FunctionDomain1DVector( x );
  return FunctionDomain1D_sptr( domain );
}

/**
 * Creates a regular n-point domain for the region on which the workspace is defined.
 */
FunctionDomain1D_sptr ChebFunction::createDomain(size_t n) const
{
  return Numeric::FunctionDomain1D_sptr( new FunctionDomain1DVector(startX(), endX(), n) );
}

/**
 * Evaluate chebfuns on a given domain
 * @param domain :: The domain
 * @param values :: The output values.
 */
void ChebFunction::eval(const FunctionDomain1D& domain, FunctionValues& values)const 
{
  const double start = startX();
  const double end   = endX();

  const size_t n = domain.size();
  size_t fi = 0;
  for(size_t i = 0; i < n; ++i)
  {
    const double x = domain[i];
    if ( x < start ) continue;
    if ( x > end ) break;
    auto f = m_fun[fi];
    while( x > f->endX() )
    {
      ++fi;
      if ( fi == m_fun.size() )
      {
        break;
      }
      f = m_fun[fi];
    }
    values.setCalculated( i, f->value( x ) );
  }
}

/// Creates a domain for the region on which the workspace is defined.
Numeric::JointDomain_sptr ChebFunction::createJointDomain() const
{
  auto jointDomain = new JointDomain;
  for(auto f = m_fun.begin(); f != m_fun.end(); ++f)
  {
    jointDomain->addDomain((**f).createDomainFromXPoints());
  }
  return JointDomain_sptr( jointDomain );
}

/**
 * Check if this workspace has the same base as another one. Having the same base means
 * that the two workspaces have the same number of chebfuns and the chebfuns with the same
 * indices have shared x-points.
 * @param other :: A function to compare with.
 */
bool ChebFunction::haveSameBase(const ChebFunction& other) const
{
  if ( nfuns() != other.nfuns() ) return false;
  for(size_t i = 0; i < nfuns(); ++i)
  {
    if ( !fun(i).haveSameBase(other.fun(i))) return false;
  }
  return true;
}

/**
 * Set this function to a constant value.
 * @param value :: A value to assign.
 */
ChebFunction& ChebFunction::operator=(const double& value)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i) = value;
  }
  return *this;
}

/**
 * Add another workspace.
 * @param other :: A workspace to add.
 */
ChebFunction& ChebFunction::operator+=(const ChebFunction& cws)
{
  if ( haveSameBase( cws ) )
  {// have shared x-points - very easy
    for(size_t i = 0; i < nfuns(); ++i)
    {
      fun(i) += cws.fun(i);
    }
  }
  else
  {// general case - more complicated
    //auto jd1 = createJointDomain();
    //auto jd2 = cws.createJointDomain();
    //DomainMap map = jd1->createDomainMap( *jd2 );
    //for(auto r = map.begin(); r != map.end(); ++r)
    //{

    //  fun( r->i1 ).apply( '+', cws.fun( r->i2 ), r->domain->as<FunctionDomain1D>() );
    //}
    throw std::runtime_error("Cannot add ChebFunction having on a different base");
  }
  return *this;
}

/**
 * Add a valut to the function.
 * @param value :: Value to add.
 */
ChebFunction& ChebFunction::operator+=(const double& value)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i) += value;
  }
  return *this;
}

/**
 * Multiply the function by a value.
 * @param value :: Value to multiply.
 */
ChebFunction& ChebFunction::operator*=(const double& value)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i) *= value;
  }
  return *this;
}

/**
 * Add another workspace.
 * @param other :: A workspace to add.
 */
ChebFunction& ChebFunction::operator-=(const ChebFunction& cws)
{
  if ( haveSameBase( cws ) )
  {// have shared x-points - very easy
    for(size_t i = 0; i < nfuns(); ++i)
    {
      fun(i) -= cws.fun(i);
    }
  }
  else
  {// general case - more complicated
    throw std::runtime_error("Cannot subtract ChebFunction having on a different base");
  }
  return *this;
}

/**
 * Add another workspace.
 * @param other :: A workspace to add.
 */
ChebFunction& ChebFunction::operator*=(const ChebFunction& cws)
{
  if ( haveSameBase( cws ) )
  {// have shared x-points - very easy
    for(size_t i = 0; i < nfuns(); ++i)
    {
      fun(i) *= cws.fun(i);
    }
  }
  else
  {// general case - more complicated
    throw std::runtime_error("Cannot multiply ChebFunction having on a different base");
  }
  return *this;
}

/**
 * Add another workspace.
 * @param other :: A workspace to add.
 */
ChebFunction& ChebFunction::operator/=(const ChebFunction& cws)
{
  if ( haveSameBase( cws ) )
  {// have shared x-points - very easy
    for(size_t i = 0; i < nfuns(); ++i)
    {
      fun(i) /= cws.fun(i);
    }
  }
  else
  {// general case - more complicated
    throw std::runtime_error("Cannot divide ChebFunction having on a different base");
  }
  return *this;
}

/**
 * Multiply by a C++ function.
 * @param f :: A function to multiply.
 */
ChebFunction& ChebFunction::operator*=(AFunction f)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i) *= f;
  }
  return *this;
}

/**
 * Multiply by a IFunction.
 * @param f :: A function to multiply.
 */
ChebFunction& ChebFunction::operator*=(const IFunction& f)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i) *= f;
  }
  return *this;
}

/**
 * Fit to a c++ function.
 * @param f :: Function to fit to.
 */
void ChebFunction::fit(AFunction f)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i).fit( f );
  }
}

/**
 * Fit to an IFunction.
 * @param ifun :: Function to fit to.
 */
void ChebFunction::fit(const IFunction& ifun)
{
  for(size_t i = 0; i < nfuns(); ++i)
  {
    fun(i).fit( ifun );
  }
}

/**
 * BestFit to an IFunction
 * @param ifun :: Function to fit to.
 */
void ChebFunction::bestFit(const IFunction& ifun)
{
  const double tol = 1e-16;
  double err = 1.0;
  std::vector<bool> done(nfuns(),false);
  std::vector<size_t> nn(nfuns(),9);
  double maxA = 0;
  while ( std::find(done.begin(),done.end(),false) != done.end() )
  {
    double minAodd = inf;
    double minAeven = inf;
    for(size_t k = 0; k < nfuns(); ++k)
    {
      if ( done[k] ) continue;
      auto& f = fun(k);
      f.set( nn[k], f.startX(), f.endX() );
      f.fit( ifun );
      auto& a = f.coeffs();
      for(size_t i = 0; i < a.size(); i+=2)
      {
        const double abs_even = fabs( a[i] );
        const double abs_odd = fabs( a[i+1] );
        if ( abs_even < minAeven ) minAeven = abs_even;
        if ( abs_odd < minAodd ) minAodd = abs_odd;
        if ( abs_even > maxA ) maxA = abs_even;
        if ( abs_odd > maxA ) maxA = abs_odd;
      }
      err = (minAodd + minAeven) / maxA / 2;
      nn[k] *= 2;
      --nn[k];
      if ( err <= tol ) done[k] = true;
    }
  }
}


/**
 * Integrate the function on the whole interval
 */
double ChebFunction::integr() const
{
  double res = 0.0;
  for(size_t i = 0; i < nfuns(); ++i)
  {
    res += fun(i).integr();
  }
  return res;
}

/**
 * make this chebfun a derivative of the argument
 * @param fun :: A function to differentiate.
 */
void ChebFunction::fromDerivative(const ChebFunction& fun)
{
  clear();
  for(size_t i = 0; i < fun.nfuns(); ++i)
  {
    auto f = new ScaledChebfun;
    f->fromDerivative( fun.fun(i) );
    m_fun.push_back( f );
  }
}

/// Square the function
void ChebFunction::square()
{
  for(size_t i = 0; i < m_fun.size(); ++i)
  {
    m_fun[i]->square();
  }
}

/// Take a square root of the function
void ChebFunction::sqrt()
{
  for(size_t i = 0; i < m_fun.size(); ++i)
  {
    m_fun[i]->sqrt();
  }
}

/**
 * Find all roots of this function.
 */
void ChebFunction::roots(std::vector<double>& r) const
{
  r.clear();
  for(size_t i = 0; i < m_fun.size(); ++i)
  {
    std::vector<double> rr;
    m_fun[i]->roots( rr );
    r.insert( r.end(), rr.begin(), rr.end() );
  }
}

} // Numeric
