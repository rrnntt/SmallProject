#ifndef NUMERIC_DOMAINMAP_H
#define NUMERIC_DOMAINMAP_H

#include "Numeric/DllExport.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Numeric
{

class FunctionDomain;

class NUMERIC_EXPORT DomainRef
{
public:
  /// Index of a domain creating this ref
  size_t i1;
  /// Index of the other domain which intersects with the domain creating this ref
  size_t i2;
  /// Domain - intersection of domains i1 and i2
  boost::shared_ptr<FunctionDomain> domain;
  DomainRef():i1(),i2(),domain(){}
  /// Constructor.
  /// @param j :: Index of the second domain. i1 = 0
  /// @param d :: Intersection domain
  DomainRef(size_t j, boost::shared_ptr<FunctionDomain> d):i1(),i2(j),domain(d){}
};

/**
 * 
 */
typedef std::vector<DomainRef> DomainMap;

} // NUMERIC

#endif // NUMERIC_DOMAINMAP_H
