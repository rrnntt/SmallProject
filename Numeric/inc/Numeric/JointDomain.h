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
  void addDomain(FunctionDomain_ptr domain);
protected:
  std::vector< FunctionDomain_ptr > m_domains;
};

} // namespace Numeric

#endif /*NUMERIC_JOINTDOMAIN_H_*/