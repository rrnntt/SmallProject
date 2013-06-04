#ifndef NUMERIC_SCALEDCHEBFUN_H
#define NUMERIC_SCALEDCHEBFUN_H

#include "Numeric/DllExport.h"
#include "Numeric/Chebfun.h"
#include "Numeric/Constants.h"

namespace Numeric
{

/**
    \class ScaledChebfun
    \brief Scales the domain to work on infinite intervals.

    If the domain is [x0, inf) or (-inf, x0] a scale function is applied to
    the chebfun's argument.

    If c is a chebfun defined in [-1,1] and s is a ScaledChebfun defined on [x0, inf) then
    s(x) = c( (x-x0-1)/(x-x0+1) ) so that
    s(x0) = c( -1 )
    s(inf) = c( 1 )

    If c is a chebfun defined in [-1,1] and s is a ScaledChebfun defined on (inf, x0] then
    s(x) = c( (-x+x0-1)/(x-x0-1) ) so that
    s(-inf) = c( -1 )
    s(x0) = c( 1 )

    A ScaledChebfun defined on [x0, x1] is the same as a chebfun on the same interval.


 */
class NUMERIC_EXPORT ScaledChebfun
{
public:
  /// Constructor
  ScaledChebfun(size_t n = 0, const double& startX = -1,const double& endX = 1);
  /// Destructor
  ~ScaledChebfun();
  /// Copy constructor.
  ScaledChebfun(const ScaledChebfun& other);
  /// Copy from a plain chebfun.
  ScaledChebfun(const chebfun& other);
  ///  Set the size and domain bounds of this function.
  void set(size_t n, const double& startX, const double& endX);
  /// get the left interval bound
  double startX() const {return m_startX;}
  /// get the right interval bound
  double endX() const {return m_endX;}
  /// Order of the polynomial
  size_t n() const {return m_fun.n();}
  /// Check if scaling is required (function is defined on an infinite interval).
  bool hasScaling() const {return m_startX == minf || m_endX == inf;}
  /// Check if two chebfuns have shared x-points.
  bool haveSameBase(const ScaledChebfun& other) const;
  /// Set same base as in other function
  void setBaseFrom(const ScaledChebfun& other);
  /// Fill a vector with unscaled x-values
  void fillXValues(std::vector<double>& x) const;
  /// Calculate function value at point x
  double value(double x) const;
  /// evaluate the function
  double operator()(const double& x)const{return value(x);}
  /// Fit to a c++ function
  void fit(AFunction f);
  /// Fit to an IFunction
  void fit(const IFunction& ifun);
  /// Best fit
  void bestFit(AFunction fun);
  /// Best fit
  void bestFit(const IFunction& ifun);
  /// make this chebfun a derivative of the argument
  void fromDerivative(const ScaledChebfun& fun);
  /// Integrate the function on the whole interval
  double integr() const;
  /// Creates a domain for the region on which the workspace is defined.
  FunctionDomain1D_sptr createDomainFromXPoints() const;
  /// Get the roots of the function
  void roots(std::vector<double>& r) const;
  const std::vector<double>& coeffs() const {return m_fun.coeffs();}
  /// Get the unscaled function
  const chebfun& getUnscaled() const {return m_fun;}

  /*=== Operations on ScaledChebfun ===*/

  /* Assignment */
  /// Make this function a copy of another
  ScaledChebfun& operator=(const ScaledChebfun& fun);
  /// Make this function a copy of a chebfun
  ScaledChebfun& operator=(const chebfun& fun);
  /// Assign values from another function to this
  ScaledChebfun& operator=(AFunction fun);
  /// Assign values from another function to this
  ScaledChebfun& operator=(const IFunction& fun);
  /// Set this function equal to a constant
  ScaledChebfun& operator=(double value);

  /* Binary */
  /// Add values from another function
  ScaledChebfun& operator+=(const ScaledChebfun& fun);
  /// Add values from another function
  ScaledChebfun& operator+=(AFunction fun);
  /// Add a value
  ScaledChebfun& operator+=(const double& value);
  /// Subtract values from another function
  ScaledChebfun& operator-=(const ScaledChebfun& fun);
  /// Subtract a value
  ScaledChebfun& operator-=(const double& value);
  /// Multiply by values from another function
  ScaledChebfun& operator*=(const ScaledChebfun& fun);
  /// Add values from another function
  ScaledChebfun& operator*=(AFunction fun);
  /// Add values from another function
  ScaledChebfun& operator*=(const IFunction& fun);
  /// Mutiply by a value
  ScaledChebfun& operator*=(const double& value);
  /// Divide by values from another function
  ScaledChebfun& operator/=(const ScaledChebfun& fun);

  /* Functions */
  /// Square the function
  void square() {m_fun.square();}
  /// Take a square root of the function
  void sqrt() {m_fun.sqrt();}
protected:
  /// Transform the argument
  double transform(double x) const;
  /// Inverse transform of the argument
  double invTransform(double t) const;
  /// Throw an invalid_argument exception after checking arguments to set(...)
  void throwInvalidArgumetns() const;
  /// Throw a runtime_error exception if ScaledChebfuns have different bases
  void throwDifferentBaseInOperation(const std::string& op) const;
  /// Get the base of the underlying chebfun
  ChebfunBase_const_sptr getBase() const {return m_fun.getBase();}

  /*------------------ Data --------------------*/

  /// Plain chebfun
  chebfun m_fun;
  /// Left bound
  double m_startX;
  /// Right bound
  double m_endX;

  friend class ChebOperator;
  friend class ChebFunction;
  template<class CHEB, typename TYPE> friend void BestFit(CHEB& cheb, TYPE fun);
};

} // NUMERIC

#endif // NUMERIC_SCALEDCHEBFUN_H
