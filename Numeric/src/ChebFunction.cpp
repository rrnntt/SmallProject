#include "Numeric/ChebFunction.h"

namespace Numeric
{

ChebFunction::ChebFunction():
m_fun(1,Numeric::chebfun_ptr(new Numeric::chebfun))
{
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
    Numeric::chebfun* f = m_fun[fi].get();
    while( x > f->endX() )
    {
      ++fi;
      if ( fi == m_fun.size() )
      {
        break;
      }
      f = m_fun[fi].get();
    }
    out[i] = f->valueB( x );
  }
}

/// Creates a domain for the region on which the workspace is defined.
Numeric::FunctionDomain1D_sptr ChebFunction::createDomainFromXPoints() const
{
  std::vector<double> x;
  size_t npts = 0;
  std::for_each(m_fun.begin(),m_fun.end(),[&npts](Numeric::chebfun_ptr f){
    npts += f->n();
  });
  ++npts;
  x.resize( npts );

  size_t i = 0; // running index
  for(auto f = m_fun.begin(); f != m_fun.end(); ++f)
  {
    auto xf = (**f).xpoints();
    // there are no gaps between chebfuns
    std::copy(xf.rbegin(), xf.rend() - 1, x.begin() + i);
    i += xf.size() - 1;
  }
  x.back() = this->endX();
  auto domain = new Numeric::FunctionDomain1DVector( x );

  return Numeric::FunctionDomain1D_sptr( domain );
}

/**
 * Creates a regular n-point domain for the region on which the workspace is defined.
 */
Numeric::FunctionDomain1D_sptr ChebFunction::createDomain(size_t n) const
{
  return Numeric::FunctionDomain1D_sptr( new Numeric::FunctionDomain1DVector(startX(), endX(), n) );
}

/**
 * Evaluate chebfuns on a given domain
 * @param domain :: The domain
 * @param values :: The output values.
 */
void ChebFunction::eval(const Numeric::FunctionDomain1D& domain, Numeric::FunctionValues& values)const 
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
    Numeric::chebfun* f = m_fun[fi].get();
    while( x > f->endX() )
    {
      ++fi;
      if ( fi == m_fun.size() )
      {
        break;
      }
      f = m_fun[fi].get();
    }
    values.setCalculated( i, f->valueB( x ) );
  }
}

/// Creates a domain for the region on which the workspace is defined.
Numeric::JointDomain_sptr ChebFunction::createJointDomain() const
{
  auto jointDomain = new Numeric::JointDomain;
  for(auto f = m_fun.begin(); f != m_fun.end(); ++f)
  {
    jointDomain->addDomain((**f).createDomainFromXPoints());
  }
  return Numeric::JointDomain_sptr( jointDomain );
}

/**
 * Check if this workspace has the same base as another one. Having the same base means
 * that the two workspaces have the same number of chebfuns and the chebfuns with the same
 * indices have shared x-points.
 * @param other :: A workspace to compare with.
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
    auto jd1 = createJointDomain();
    auto jd2 = cws.createJointDomain();
    Numeric::DomainMap map = jd1->createDomainMap( *jd2 );
    for(auto r = map.begin(); r != map.end(); ++r)
    {

      fun( r->i1 ).apply( '+', cws.fun( r->i2 ), r->domain->as<Numeric::FunctionDomain1D>() );
    }
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
    auto jd1 = createJointDomain();
    auto jd2 = cws.createJointDomain();
    Numeric::DomainMap map = jd1->createDomainMap( *jd2 );
    for(auto r = map.begin(); r != map.end(); ++r)
    {

      fun( r->i1 ).apply( '-', cws.fun( r->i2 ), r->domain->as<Numeric::FunctionDomain1D>() );
    }
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
    auto jd1 = createJointDomain();
    auto jd2 = cws.createJointDomain();
    Numeric::DomainMap map = jd1->createDomainMap( *jd2 );
    for(auto r = map.begin(); r != map.end(); ++r)
    {

      fun( r->i1 ).apply( '*', cws.fun( r->i2 ), r->domain->as<Numeric::FunctionDomain1D>() );
    }
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
    auto jd1 = createJointDomain();
    auto jd2 = cws.createJointDomain();
    Numeric::DomainMap map = jd1->createDomainMap( *jd2 );
    for(auto r = map.begin(); r != map.end(); ++r)
    {

      fun( r->i1 ).apply( '/', cws.fun( r->i2 ), r->domain->as<Numeric::FunctionDomain1D>() );
    }
  }
  return *this;
}

} // Numeric
