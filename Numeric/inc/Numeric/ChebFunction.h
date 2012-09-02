#ifndef NUMERIC_CHEBFUNCTION_H
#define NUMERIC_CHEBFUNCTION_H

#include "Numeric/DllExport.h"
#include "Numeric/ScaledChebfun.h"
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
  /// Copy constructor
  ChebFunction(const ChebFunction& other, bool copy = true);
  /// Create and init with a single chebfun
  ChebFunction(const chebfun& fun);
  /// Create and init with a single empty chebfun
  ChebFunction(size_t n, const double& startX,const double& endX);
  /// Destructor.
  ~ChebFunction();
  /// Returns the function's name
  virtual std::string name()const {return "ChebFunction";}
  /// Function you want to fit.
  virtual void function1D(double* out, const double* xValues, const size_t nData)const;
  /// Start of the domain
  double startX()const {return m_fun.front()->startX();}
  /// End of the domain
  double endX()const{return m_fun.back()->endX();}
  /// Number of chebfuns in the workspace
  size_t nfuns() const {return m_fun.size();}
  /// Return reference to i-th chebfun, const version
  const ScaledChebfun& fun(size_t i) const {return *(m_fun[i]);}
  /// Add a ScaledChebfun to the right on the x-axis.
  void appendRight(size_t n, const double& endX);
  /// Fill a vector with x values
  void fillXValues(std::vector<double>& x) const;
  /// Creates a domain for the region on which the function is defined.
  FunctionDomain1D_sptr createDomainFromXPoints() const;
  /// Creates a domain for the region on which the function is defined.
  FunctionDomain1D_sptr createDomain(size_t n) const;
  /// Creates a domain for the region on which the function is defined.
  JointDomain_sptr createJointDomain() const;
  /// Evaluate chebfuns on a given domain
  void eval(const FunctionDomain1D& domain, FunctionValues& values)const ;
  /// Check if this function has the same base as another one
  bool haveSameBase(const ChebFunction& other) const;
  /// Fit to a c++ function
  void fit(AFunction f);
  /// Fit to an IFunction
  void fit(const IFunction& ifun);
  /// BestFit to an IFunction
  void bestFit(const IFunction& ifun);
  /// make this chebfun a derivative of the argument
  void fromDerivative(const ChebFunction& fun);
  /// Integrate the function on the whole interval
  double integr() const;

  /// Performs a binary operation
  void binaryOperation(const ChebFunction& cws, const char op);

  ChebFunction& operator=(const ChebFunction& cws);
  ChebFunction& operator=(const double& value);
     /* Binary operators */
  ChebFunction& operator+=(const ChebFunction& cws);
  ChebFunction& operator-=(const ChebFunction& cws);
  ChebFunction& operator*=(const ChebFunction& cws);
  ChebFunction& operator/=(const ChebFunction& cws);

  ChebFunction& operator+=(const double& value);
  ChebFunction& operator*=(const double& value);

  ChebFunction& operator*=(AFunction fun);

  /* Functions */
  /// Square the function
  void square();
  /// Take a square root of the function
  void sqrt();

protected:
  /// Return reference to i-th chebfun
  ScaledChebfun& fun(size_t i) {return *(m_fun[i]);}
  /// clear all data
  void clear();
  /// Vector of chebfuns
  std::vector<ScaledChebfun*> m_fun;
};

/// typedef shared pointer
typedef boost::shared_ptr<ChebFunction> ChebFunction_sptr;
typedef boost::shared_ptr<const ChebFunction> ChebFunction_const_sptr;

} // NUMERIC

#endif // NUMERIC_CHEBFUNCTION_H
