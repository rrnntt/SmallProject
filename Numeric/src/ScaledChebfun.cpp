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
 * Copy from a plain chebfun. This function will be unscaled and equal to cheb.
 * @param cheb :: A chebfun to copy from
 */
ScaledChebfun::ScaledChebfun(const chebfun& cheb)
:m_fun(cheb),m_startX(cheb.startX()),m_endX(cheb.endX())
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
 * Set same base as in other function
 * @param other :: Function to share base with.
 */
void ScaledChebfun::setBaseFrom(const ScaledChebfun& other)
{
  m_fun.setBase( other.m_fun );
  m_startX = other.startX();
  m_endX = other.endX();
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
 * Fill a vector with unscaled x-values. First or last value can be infinite.
 * @param x : A vector to fill.
 */
void ScaledChebfun::fillXValues(std::vector<double>& x) const
{
  auto& xx = getBase()->x;
  x.resize(xx.size());
  for(size_t i = 0; i < xx.size(); ++i)
  {
    x[i] = invTransform( xx[i] );
  }
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

/**
 * Fit to an IFunction. 
 * @param ifun :: Function to fit to. This chebfun will approximate f.
 */
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

/**
 * Make this function a copy of another
 * @param fun :: 
 */
ScaledChebfun& ScaledChebfun::operator=(const ScaledChebfun& fun)
{
  m_startX = fun.m_startX;
  m_endX = fun.m_endX;
  m_fun = fun.m_fun;
  return *this;
}

/**
 * Make this function a copy of a chebfun
 * @param fun :: A chebfun to copy from.
 */
ScaledChebfun& ScaledChebfun::operator=(const chebfun& fun)
{
  m_startX = fun.startX();
  m_endX = fun.endX();
  m_fun = fun;
  return *this;
}

/**
 * Assign values from another function to this
 * @param fun :: 
 */
ScaledChebfun& ScaledChebfun::operator=(AFunction fun)
{
  fit( fun );
  return *this;
}

/**
 * Assign values from another function to this
 * @param fun :: 
 */
ScaledChebfun& ScaledChebfun::operator=(const IFunction& fun)
{
  fit( fun );
  return *this;
}

/**
 * Set this function equal to a constant
 * @param value :: A constant value to set.
 */
ScaledChebfun& ScaledChebfun::operator=(double value)
{
  m_fun = value;
  return *this;
}


/**
 * Throw a runtime_error exception if ScaledChebfuns have different bases
 * @param op :: Name of an operation requiring the same base for the ScaledChebfuns invloved.
 */
void ScaledChebfun::throwDifferentBaseInOperation(const std::string& op) const
{
  throw std::runtime_error("ScaledChebfun operation \""+op+"\" requires equal bases");
}

/**
 * Add values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator+=(const ScaledChebfun& fun)
{
  if ( !haveSameBase( fun ) ) throwDifferentBaseInOperation("+");
  m_fun += fun.m_fun;
  return *this;
}

/**
 * Add values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator+=(AFunction fun)
{
  if ( !hasScaling() )
  {
    m_fun += fun;
  }
  else
  {
    std::vector<double> x;
    fillXValues( x );
    std::vector<double> y( x.size() );
    // copy m_p values to y
    y = m_fun.ypoints();
    size_t istart = 0;
    size_t ni = x.size();
    if ( ni > 1 )
    {
      if ( x.front() == minf )
      {
        istart = 1;
        --ni;
      }
      else if ( x.back() == inf )
      {
        --ni;
      }
    }
    for(size_t i = istart; i < ni; ++i)
    {
      y[i] += fun( x[i] );
    }
    m_fun.setP( y );
  }
  return *this;
}

/**
 * Add a value
 * @param value :: Value to add.
 */
ScaledChebfun& ScaledChebfun::operator+=(const double& value)
{
  m_fun += value;
  return *this;
}

/**
 * Subtract values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator-=(const ScaledChebfun& fun)
{
  if ( !haveSameBase( fun ) ) throwDifferentBaseInOperation("-");
  m_fun -= fun.m_fun;
  return *this;
}

/**
 * Subtract a value
 * @param value :: Value to subtract.
 */
ScaledChebfun& ScaledChebfun::operator-=(const double& value)
{
  m_fun -= value;
  return *this;
}


/**
 * Multiply by values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator*=(const ScaledChebfun& fun)
{
  if ( !haveSameBase( fun ) ) throwDifferentBaseInOperation("*");
  m_fun *= fun.m_fun;
  return *this;
}

/**
 * Multiply by values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator*=(AFunction fun)
{
  if ( !hasScaling() )
  {
    m_fun *= fun;
  }
  else
  {
    std::vector<double> x;
    fillXValues( x );
    std::vector<double> y( x.size() );
    // copy m_p values to y
    y = m_fun.ypoints();
    size_t istart = 0;
    size_t ni = x.size();
    if ( ni > 1 )
    {
      if ( x.front() == minf )
      {
        istart = 1;
        --ni;
      }
      else if ( x.back() == inf )
      {
        --ni;
      }
    }
    for(size_t i = istart; i < ni; ++i)
    {
      y[i] *= fun( x[i] );
    }
    m_fun.setP( y );
  }
  return *this;
}

/**
 * Multiply by a value
 * @param value :: Value to multiply by.
 */
ScaledChebfun& ScaledChebfun::operator*=(const double& value)
{
  m_fun *= value;
  return *this;
}


/**
 * Divide by values from another function
 * @param fun :: A function to add to this function.
 */
ScaledChebfun& ScaledChebfun::operator/=(const ScaledChebfun& fun)
{
  if ( !haveSameBase( fun ) ) throwDifferentBaseInOperation("/");
  m_fun /= fun.m_fun;
  return *this;
}

/**
 * make this chebfun a derivative of the argument
 */
void ScaledChebfun::fromDerivative(const ScaledChebfun& fun)
{
  m_startX = fun.m_startX;
  m_endX = fun.m_endX;
  m_fun.fromDerivative( fun.m_fun );
  if ( !hasScaling() ) return;
  // use the chain rule to calculate the derivatives
  std::vector<double> x;
  fillXValues( x );
  // copy m_p values to y
  std::vector<double> y = m_fun.ypoints();
  for(size_t i = 0; i < x.size(); ++i)
  {
    double xx = m_endX == inf ? x[i] - m_startX + 1.0 : -x[i] + m_endX + 1.0;
    y[i] *= 2.0 / ( xx*xx );
  }
  m_fun.setP( y );
}

namespace
{
  double jacobian_plus(double x)
  {
    if ( x == 1.0 ) return 0.0;
    const double xx = 1.0 - x;
    return 2.0 / ( xx*xx );
  }
  double jacobian_minus(double x)
  {
    if ( x == -1.0 ) return 0.0;
    const double xx = 1.0 + x;
    return 2.0 / ( xx*xx );
  }
}

/**
 * Integrate the function on the whole interval
 */
double ScaledChebfun::integr() const
{
  if ( !hasScaling() ) return m_fun.integr();
  chebfun tmp = m_fun;
  if ( m_endX == inf )
  {
    tmp *= jacobian_plus;
  }
  else
  {
    tmp *= jacobian_minus;
  }
  return tmp.integr();
}

/**
 * Creates a domain for the region on which the workspace is defined.
 */
FunctionDomain1D_sptr ScaledChebfun::createDomainFromXPoints() const
{
  std::vector<double> x;
  fillXValues( x );
  auto domain = new Numeric::FunctionDomain1DVector( x );

  return Numeric::FunctionDomain1D_sptr( domain );
}

/**
  * Find the number on x-points (n) large enough to reproduce fun
  * with accuracy ~1e-16.
  * @param fun :: Function to fit to.
  */
void ScaledChebfun::bestFit(AFunction fun)
{
  BestFit( *this, fun );
}

/**
  * Find the number on x-points (n) large enough to reproduce fun
  * with accuracy ~1e-16.
  * @param fun :: Function to fit to.
  */
void ScaledChebfun::bestFit(const IFunction& fun)
{
  BestFit<ScaledChebfun,const IFunction&>( *this, fun );
}

} // Numeric
