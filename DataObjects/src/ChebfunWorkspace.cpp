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
 * Add another workspace
 */
ChebfunWorkspace& ChebfunWorkspace::operator+=(const ChebfunWorkspace& cws)
{
  auto jd1 = createJointDomain();
  auto jd2 = cws.createJointDomain();
  size_t i2 = 0;
  for(size_t i1 = 0; i1 < jd1->getNParts(); ++i1)
  {
    auto& jd = jd1->getDomain(i1).as<Numeric::FunctionDomain1D>();
  }
  return *this;
}


} // namespace DataObjects
