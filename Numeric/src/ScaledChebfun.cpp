#include "Numeric/ScaledChebfun.h"
#include "Numeric/IFunction.h"

#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <vector>

namespace Numeric
{

/**
 * Constructs a ScaledChebfun with n points in the inerval [startX, endX]. startX must be
 * smaller than endX. If endX is Numeric::inf (positive infinity) then the function is
 * defined in the [startX, inf) interval, startX cannot be Numeric::minf (negative infinity) in this case.
 * If startX is Numeric::minf (negative infinity) then the function is
 * defined in the (-inf, endX] interval, endX cannot be Numeric::inf (positive infinity) in this case.
 * @param n :: Number of points in the interval. 
 * @param startX :: The left interval boundary. 
 * @param endX :: The right interval boundary. 
 */
ScaledChebfun::ScaledChebfun(size_t n, const double& startX, const double& endX)
{
  if ( n >= 0 )
  {
    set( n, startX, endX );
  }
}

/**
 * Copy constructor. Copies the data from another ScaledChebfun, shares ChebfunBase
 * @param other :: ScaledChebfun to copy from.
 */
ScaledChebfun::ScaledChebfun(const ScaledChebfun& other):
m_fun(other.m_fun),m_startX(other.m_startX),m_endX(other.m_endX)
{
}

/**
 * Constructs a ScaledChebfun with n points in the inerval [startX, endX]. startX must be
 * smaller than endX. If endX is Numeric::inf (positive infinity) then the function is
 * defined in the [startX, inf) interval, startX cannot be Numeric::minf (negative infinity) in this case.
 * If startX is Numeric::minf (negative infinity) then the function is
 * defined in the (-inf, endX] interval, endX cannot be Numeric::inf (positive infinity) in this case.
 * @param n :: Number of points in the interval. 
 * @param startX :: The left interval boundary. 
 * @param endX :: The right interval boundary. 
*/
void ScaledChebfun::set(size_t n, const double& startX, const double& endX)
{
  m_startX = startX;
  m_endX = endX;

  if ( fabs( startX ) != inf && fabs( endX ) != inf && startX < endX )
  {// finite interval [startX, endX]
    m_fun.set( n, startX, endX );
  }
  else if ( startX == minf && fabs( endX ) != inf )
  {// infinite interval (-inf, endX]
    m_fun.set( n, -1, 1 );
  }
  else if ( endX == inf && fabs( startX ) != inf )
  {// infinite interval [startX, inf)
    m_fun.set( n, -1, 1 );
  }
  else
  {
    throwInvalidArgumetns();
  }
}

/**
 * Destructor.
 */
ScaledChebfun::~ScaledChebfun()
{
}

/**
 * Check if two chebfuns have shared x-points.
 */
bool ScaledChebfun::haveSameBase(const ScaledChebfun& other) const
{
  if ( m_startX != other.startX() || m_endX != other.endX() ) return false;
  return m_fun.haveSameBase( other.m_fun );
}

/**
 * Throw an invalid_argument exception after checking arguments to set(...)
 */
void ScaledChebfun::throwInvalidArgumetns() const
{
    throw std::invalid_argument( "ScaledChebfun cannot be created on this interval:\n"
      +boost::lexical_cast<std::string>(m_startX)+", "+boost::lexical_cast<std::string>(m_endX));
}

/**
 * Transform the argument if one of the bounds is infinity.
 * @param x :: An untransformed argument.
 * @return :: Transformed value to use in the plain chebfun m_fun.
 */
double ScaledChebfun::transform(double x) const
{
  if ( m_startX == minf )
  {
    if ( fabs( x ) < 1.0 )
    {
      return (-x + m_endX - 1.0 )/(x - m_endX - 1.0);
    }
    else
    {
      return ( -1.0 + (m_endX - 1.0)/x )/( 1.0 - (m_endX + 1.0)/x );
    }
  }
  else if ( m_endX == inf )
  {
    if ( fabs( x ) < 1.0 )
    {
      return (x - m_startX - 1.0)/(x - m_startX + 1.0);
    }
    else
    {
      return ( 1.0 - (m_startX + 1.0)/x )/( 1.0 - (m_startX - 1.0)/x );
    }
  }
  return x;
}

/**
 * Inverse transform of the argument.
 * @param t :: Argument of the plain chebfun.
 */
double ScaledChebfun::invTransform(double t) const
{
  if ( m_startX == minf )
  {
    if ( t == -1.0 ) return minf;
    return ((m_endX + 1.0)*t + m_endX - 1.0)/(t + 1.0);
  }
  else if ( m_endX == inf )
  {
    if ( t == 1.0 ) return inf;
    return ((m_startX - 1.0)*t - m_startX - 1.0)/(t - 1.0);
  }
  return t;
}

/**
 * Calculate function value at point x.
 * @param x :: Point at which to calculate the function.
 */
double ScaledChebfun::value(double x) const
{
  return m_fun( transform(x) );
}

/**
 * Fit to a c++ function. 
 * @param f :: Function to fit to. This chebfun will approximate f.
 */
void ScaledChebfun::fit(AFunction f)
{
  if ( !hasScaling() )
  {
    m_fun.fit( f );
    return;
  }
  auto & x = m_fun.getBase()->x;
  std::vector<double> y( x.size() );
  for(size_t i = 0; i < x.size(); ++i)
  {
    y[i] = f( invTransform(x[i]) );
  }
  m_fun.setP( y );
}

/// Fit to an IFunction
void ScaledChebfun::fit(const IFunction& ifun)
{
  if ( !hasScaling() )
  {
    m_fun.fit( ifun );
    return;
  }
  auto& x = getBase()->x;
  std::vector<double> xx(x.size());
  for(size_t i = 0; i < xx.size(); ++i)
  {
    xx[i] = invTransform( x[i] );
  }
  FunctionDomain1DView domain(xx.data(),xx.size());
  FunctionValues values(domain);
  ifun.function(domain,values);
  std::vector<double> y(xx.size());
  for(size_t i=0; i < x.size(); ++i)
  {
    y[i] = values.getCalculated(i);
  }
  m_fun.setP( y );
}


} // Numeric
