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
  /// Calculate function value at point x
  double value(double x) const;
  /// Fit to a c++ function
  void fit(AFunction f);
  /// Fit to an IFunction
  void fit(const IFunction& ifun);
protected:
  /// Transform the argument
  double transform(double x) const;
  /// Inverse transform of the argument
  double invTransform(double t) const;
  /// Throw an invalid_argument exception after checking arguments to set(...)
  void throwInvalidArgumetns() const;
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
};

} // NUMERIC

#endif // NUMERIC_SCALEDCHEBFUN_H
