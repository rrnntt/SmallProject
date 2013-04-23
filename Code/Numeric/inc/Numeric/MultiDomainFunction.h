#ifndef NUMERIC_MULTIDOMAINFUNCTION_H_
#define NUMERIC_MULTIDOMAINFUNCTION_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "Numeric/CompositeFunction.h"

#include <map>

namespace Numeric
{
  class CompositeDomain;
/** A composite function defined on a CompositeDomain. Member functions can be applied to 
    one or more member domains of the CompositeDomain. If two functions applied to the same domain
    the results are added (+).

*/
class NUMERIC_EXPORT MultiDomainFunction : public CompositeFunction
{
public:
  /// Constructor
  MultiDomainFunction():m_nDomains(0),m_maxIndex(0){}

  /// Returns the function's name
  virtual std::string name()const {return "MultiDomainFunction";}
  /// Function you want to fit to. 
  /// @param domain :: The buffer for writing the calculated values. Must be big enough to accept dataSize() values
  virtual void function(const FunctionDomain& domain, FunctionValues& values)const;
  /// Derivatives of function with respect to active parameters
  virtual void functionDeriv(const FunctionDomain& domain, Jacobian& jacobian);

  /// Associate a function and a domain
  void setDomainIndex(size_t funIndex, size_t domainIndex);

  /// Associate a function and a list of domains
  void setDomainIndices(size_t funIndex, const std::vector<size_t>& domainIndices);

  /// Clear all domain indices
  void clearDomainIndices();

  /// Returns the number of attributes associated with the function
  virtual size_t nLocalAttributes()const {return 1;}
  /// Returns a list of attribute names
  virtual std::vector<std::string> getLocalAttributeNames()const {return std::vector<std::string>(1,"domains");}
  /// Return a value of attribute attName
  virtual Attribute getLocalAttribute(size_t i, const std::string& attName)const;
  /// Set a value to attribute attName
  virtual void setLocalAttribute(size_t i, const std::string& attName,const Attribute& );
  /// Check if attribute attName exists
  virtual bool hasLocalAttribute(const std::string& attName)const {return attName == "domains";}

protected:

  /// Counts number of the domains
  void countNumberOfDomains();
  void countValueOffsets(const CompositeDomain& domain)const;
  void getFunctionDomains(size_t i, const CompositeDomain& cd, std::vector<size_t>& domains)const;

  /// Domain index map: finction -> domain
  std::map<size_t, std::vector<size_t> > m_domains;
  /// Number of domains this MultiDomainFunction operates on. == number of different values in m_domains
  size_t m_nDomains;
  /// Maximum domain index
  size_t m_maxIndex;
  mutable std::vector<size_t> m_valueOffsets;
};

} // namespace Numeric

#endif /*NUMERIC_MULTIDOMAINFUNCTION_H_*/
