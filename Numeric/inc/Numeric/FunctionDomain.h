#ifndef NUMERIC_FUNCTIONDOMAIN_H_
#define NUMERIC_FUNCTIONDOMAIN_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"

#include <boost/shared_ptr.hpp>

namespace Numeric
{
/** Base class that represents the domain of a function.
    A domain is a generalisation of x (argument) and y (value) arrays.
    A domain consists at least of a list of function arguments for which a function should 
    be evaluated and a buffer for the calculated values. If used in fitting also contains
    the fit data and weights.
*/
class NUMERIC_EXPORT FunctionDomain
{
public:
  /// Virtual destructor
  virtual ~FunctionDomain(){}
  /// Return the number of points in the domain
  virtual size_t size() const  = 0;
  /// Reset the the domain so it can be reused. Implement this method for domains with a state.
  virtual void reset() const {}
};

/// typedef for a shared pointer
typedef boost::shared_ptr<FunctionDomain> FunctionDomain_sptr;

} // namespace Numeric

#endif /*NUMERIC_FUNCTIONDOMAIN_H_*/
