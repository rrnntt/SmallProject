#include "Numeric/FunctionDomain.h"

namespace Numeric
{

/// Create a map of domains intersecting with this
DomainMap FunctionDomain::createDomainMap(const JointDomain&) const
{
  return DomainMap();
}

} // namespace Numeric
