//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/JointDomain.h"

#include <algorithm>

namespace Numeric
{
  /// Return the number of points in the domain
  size_t JointDomain::size() const
  {
    size_t n = 0;
    std::for_each(m_domains.begin(),m_domains.end(),[&n](const FunctionDomain_sptr d){
      n += d->size();
    });
    return n;
  }
  /// Return the number of parts in the domain
  size_t JointDomain::getNParts() const
  {
    return m_domains.size();
  }
  /// Return i-th domain
  const FunctionDomain& JointDomain::getDomain(size_t i) const
  {
    return *m_domains.at(i);
  }
  void JointDomain::addDomain(FunctionDomain_sptr domain)
  {
    m_domains.push_back(domain);
  }

  /**
   * Create a map of domains intersecting with this
   * @param jointDomain :: A domain to compare with
   */
  DomainMap JointDomain::createDomainMap(const JointDomain& jointDomain) const
  {
    DomainMap map;
    for(auto d = m_domains.begin(); d != m_domains.end(); ++d)
    {
      DomainMap m = (**d).createDomainMap( jointDomain );
      for(auto it = m.begin(); it != m.end(); ++it)
      {
        it->i1 = static_cast<size_t>(it - m.begin());
        map.push_back( *it );
      }
    }
    return map;
  }

  
} // namespace Numeric
