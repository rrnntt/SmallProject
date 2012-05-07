#ifndef NUMERIC_FUNCTIONDOMAIN_H_
#define NUMERIC_FUNCTIONDOMAIN_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/DllExport.h"
#include "Numeric/DomainMap.h"

#include <boost/shared_ptr.hpp>
#include <vector>

namespace Numeric
{

class JointDomain;

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
  /// Create a map of domains intersecting with this
  virtual DomainMap createDomainMap(const JointDomain&) const;
  /// Cast domain to a particular type
  template<class T>
  T& as()
  {
    T* p = dynamic_cast<T*>(this);
    const std::string typeName(typeid(T).name());
    std::string ttt = "Cannot convert FunctionDomain to ";
    ttt += typeName;
    if ( !p ) throw std::runtime_error(ttt);
    return *p;
  }
  /// Cast domain to a particular const type
  template<class T>
  const T& as() const
  {
    const T* p = dynamic_cast<const T*>(this);
    const std::string typeName(typeid(T).name());
    std::string ttt = "Cannot convert FunctionDomain to ";
    ttt += typeName;
    if ( !p ) throw std::runtime_error(ttt);
    return *p;
  }
};

/// typedef for a shared pointer
typedef boost::shared_ptr<FunctionDomain> FunctionDomain_sptr;


} // namespace Numeric

#endif /*NUMERIC_FUNCTIONDOMAIN_H_*/
