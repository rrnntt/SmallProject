#ifndef NUMERIC_COMPOSITEDOMAIN_H_
#define NUMERIC_COMPOSITEDOMAIN_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/FunctionDomain.h"

#include <stdexcept>

namespace Numeric
{

class NUMERIC_EXPORT CompositeDomain: public FunctionDomain
{
public:
  /// Return the number of parts in the domain
  virtual size_t getNParts() const  = 0;
  /// Return i-th domain
  virtual const FunctionDomain& getDomain(size_t i) const = 0;
};

} // namespace Numeric

#endif /*NUMERIC_COMPOSITEDOMAIN_H_*/
