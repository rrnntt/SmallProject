#ifndef DATAOBJECTS_CHEBFUNWORKSPACE_H
#define DATAOBJECTS_CHEBFUNWORKSPACE_H

#include "DataObjects/DllExport.h"
#include "API/Workspace.h"
#include "Numeric/Chebfun.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/JointDomain.h"

#include <vector>

namespace DataObjects
{

/**
 * 
 */
class DATAOBJECTS_EXPORT ChebfunWorkspace: public API::Workspace
{
public:
  ChebfunWorkspace();
  ~ChebfunWorkspace(){}
  virtual std::string id()const {return "ChebfunWorkspace";}
  /// Start of the domain
  double startX()const {return m_fun.front()->startX();}
  /// End of the domain
  double endX()const{return m_fun.back()->endX();}
  /// Number of chebfuns in the workspace
  size_t nfuns() const {return m_fun.size();}
  /// Return reference to i-th chebfun
  Numeric::chebfun& fun(size_t i) {return *(m_fun[i]);}
  /// Return reference to i-th chebfun, const version
  const Numeric::chebfun& fun(size_t i) const {return *(m_fun[i]);}
  /// Creates a domain for the region on which the workspace is defined.
  Numeric::FunctionDomain1D_sptr createDomainFromXPoints() const;
  /// Creates a domain for the region on which the workspace is defined.
  Numeric::FunctionDomain1D_sptr createDomain(size_t n) const;
  /// Creates a domain for the region on which the workspace is defined.
  Numeric::JointDomain_sptr createJointDomain() const;
  /// Evaluate chebfuns on a given domain
  void function(const Numeric::FunctionDomain1D& domain, Numeric::FunctionValues& values)const ;
  /// Check if this workspace has the same base as another one
  bool haveSameBase(const ChebfunWorkspace& other) const;
  /// Performs a binary operation
  void binaryOperation(const ChebfunWorkspace& cws, const char op);
     /* Binary operators */
  ChebfunWorkspace& operator+=(const ChebfunWorkspace& cws);
  ChebfunWorkspace& operator-=(const ChebfunWorkspace& cws);
  ChebfunWorkspace& operator*=(const ChebfunWorkspace& cws);
  ChebfunWorkspace& operator/=(const ChebfunWorkspace& cws);
protected:
  std::vector<Numeric::chebfun_ptr> m_fun;
};

/// typedef shared pointer
typedef boost::shared_ptr<ChebfunWorkspace> ChebfunWorkspace_sptr;
typedef boost::shared_ptr<const ChebfunWorkspace> ChebfunWorkspace_const_sptr;

} // namespace DataObjects
#endif // DATAOBJECTS_CHEBFUNWORKSPACE_H
