#include "DataObjects/ChebfunWorkspace.h"
#include "API/WorkspaceFactory.h"

#include <algorithm>

namespace DataObjects
{

DECLARE_WORKSPACE(ChebfunWorkspace);

ChebfunWorkspace::ChebfunWorkspace():
m_fun(1,Numeric::chebfun_ptr(new Numeric::chebfun))
{
}

/// Creates a domain for the region on which the workspace is defined.
Numeric::FunctionDomain1D_sptr ChebfunWorkspace::createDomainFromXPoints() const
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
Numeric::FunctionDomain1D_sptr ChebfunWorkspace::createDomain(size_t n) const
{
  return Numeric::FunctionDomain1D_sptr( new Numeric::FunctionDomain1DVector(startX(), endX(), n) );
}

/**
 * Evaluate chebfuns on a given domain
 * @param domain :: The domain
 * @param values :: The output values.
 */
void ChebfunWorkspace::function(const Numeric::FunctionDomain1D& domain, Numeric::FunctionValues& values)const 
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
Numeric::JointDomain_sptr ChebfunWorkspace::createJointDomain() const
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
bool ChebfunWorkspace::haveSameBase(const ChebfunWorkspace& other) const
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
ChebfunWorkspace& ChebfunWorkspace::operator+=(const ChebfunWorkspace& cws)
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
    size_t i2 = 0;
    Numeric::FunctionDomain1D_sptr subd;
    for(size_t i1 = 0; i1 < jd1->getNParts(); ++i1)
    {
      auto& d1d1 = jd1->getDomain(i1).as<Numeric::FunctionDomain1D>();
      const double start = d1d1.startX();
      const double end = d1d1.endX();
      auto& d1d2 = jd2->getDomain(i2).as<Numeric::FunctionDomain1D>();
      double from = d1d2.startX();
      double to = d1d2.endX();
      
    }
  }
  return *this;
}


} // namespace DataObjects
