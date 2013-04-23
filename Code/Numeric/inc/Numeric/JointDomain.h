#ifndef NUMERIC_JOINTDOMAIN_H_
#define NUMERIC_JOINTDOMAIN_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/CompositeDomain.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

namespace Numeric
{

class NUMERIC_EXPORT JointDomain: public CompositeDomain
{
public:
  /// Return the number of points in the domain
  virtual size_t size() const;
  /// Return the number of parts in the domain
  virtual size_t getNParts() const;
  /// Return i-th domain
  virtual const FunctionDomain& getDomain(size_t i) const;
  void addDomain(FunctionDomain_sptr domain);
  /// Create a map of domains intersecting with this
  DomainMap createDomainMap(const JointDomain&) const;
protected:
  std::vector< FunctionDomain_sptr > m_domains;
};

typedef boost::shared_ptr<JointDomain> JointDomain_sptr;

} // namespace Numeric

#endif /*NUMERIC_JOINTDOMAIN_H_*/
