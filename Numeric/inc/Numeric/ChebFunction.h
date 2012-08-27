#ifndef NUMERIC_CHEBFUNCTION_H
#define NUMERIC_CHEBFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/Chebfun.h"
#include "Numeric/FunctionDomain1D.h"
#include "Numeric/FunctionValues.h"
#include "Numeric/JointDomain.h"
#include "Numeric/IFunction1D.h"
#include "Numeric/ParamFunction.h"

#include <vector>

namespace Numeric
{

  class PrivateData;

/**
  @class ChebFunction
  


 */
class NUMERIC_EXPORT ChebFunction: public IFunction1D, public ParamFunction
{
public:
  ChebFunction();
  ~ChebFunction();
  /// Returns the function's name
  virtual std::string name()const {return "ChebFunction";}
  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Start of the domain
  double startX()const {return m_fun.front().fun->startX();}
  /// End of the domain
  double endX()const{return m_fun.back().fun->endX();}
  /// Number of chebfuns in the workspace
  size_t nfuns() const {return m_fun.size();}
  /// Return reference to i-th chebfun
  Numeric::chebfun& fun(size_t i) {return *(m_fun[i].fun);}
  /// Return reference to i-th chebfun, const version
  const Numeric::chebfun& fun(size_t i) const {return *(m_fun[i].fun);}
  /// Creates a domain for the region on which the function is defined.
  Numeric::FunctionDomain1D_sptr createDomainFromXPoints() const;
  /// Creates a domain for the region on which the function is defined.
  Numeric::FunctionDomain1D_sptr createDomain(size_t n) const;
  /// Creates a domain for the region on which the function is defined.
  Numeric::JointDomain_sptr createJointDomain() const;
  /// Evaluate chebfuns on a given domain
  void eval(const FunctionDomain1D& domain, FunctionValues& values)const ;
  /// Check if this function has the same base as another one
  bool haveSameBase(const ChebFunction& other) const;
  /// Performs a binary operation
  void binaryOperation(const ChebFunction& cws, const char op);
     /* Binary operators */
  ChebFunction& operator+=(const ChebFunction& cws);
  ChebFunction& operator-=(const ChebFunction& cws);
  ChebFunction& operator*=(const ChebFunction& cws);
  ChebFunction& operator/=(const ChebFunction& cws);
protected:
  struct Storage
  {
    Storage(chebfun* f):fun(f){}
    chebfun* fun;
  };
  /// Evaluate a chebfun
  double eval(Storage& s, double x) const;
  /// Vector of chebfuns
  std::vector<Storage> m_fun;
};

/// typedef shared pointer
typedef boost::shared_ptr<ChebFunction> ChebFunction_sptr;
typedef boost::shared_ptr<const ChebFunction> ChebFunction_const_sptr;

} // NUMERIC

#endif // NUMERIC_CHEBFUNCTION_H
