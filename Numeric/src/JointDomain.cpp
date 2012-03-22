//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/JointDomain.h"

namespace Numeric
{
  /// Return the number of points in the domain
  size_t JointDomain::size() const
  {
    size_t n = 0;
    std::for_each(m_domains.begin(),m_domains.end(),[&n](const FunctionDomain_ptr d){
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
  void JointDomain::addDomain(FunctionDomain_ptr domain)
  {
    m_domains.push_back(domain);
  }
  
} // namespace Numeric
