#include "Numeric/ChFun.h"
#include "Numeric/FunctionFactory.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

  DECLARE_FUNCTION(ChFun);

/**
 * Constructor.
 */
ChFun::ChFun(size_t n, const double& startX,const double& endX):
chebfun(n,startX,endX),IFunction1D()
{
  m_errors.resize(n + 1);
  m_isFixed.resize( n + 1 );
  m_explicitlySet.resize( n + 1 );
  declareAttribute("n", Attribute(3));
  declareAttribute("StartX", Attribute(-1.0));
  declareAttribute("EndX", Attribute(1.0));
}

ChFun::ChFun(const ChFun& fun):
chebfun(fun)
{
  m_errors.assign(fun.m_errors.cbegin(),m_errors.cend());
  m_isFixed.assign(m_isFixed.cbegin(),m_isFixed.cend());
  m_explicitlySet.assign(m_explicitlySet.cbegin(),m_explicitlySet.cend());
}

ChFun& ChFun::operator=(const ChFun& fun)
{
  chebfun::operator=(fun);
  m_errors.assign(fun.m_errors.cbegin(),m_errors.cend());
  m_isFixed.assign(m_isFixed.cbegin(),m_isFixed.cend());
  m_explicitlySet.assign(m_explicitlySet.cbegin(),m_explicitlySet.cend());
  return *this;
}

/**
 * Destructor.
 */
ChFun::~ChFun()
{
}

void ChFun::function1D(double* out, const double* xValues, const size_t nData)const
{
  const double start = startX();
  const double end   = endX();
  for(size_t i = 0; i < nData; ++i)
  {
    const double x = xValues[i];
    if ( x < start ) continue;
    if ( x > end ) break;
    out[i] = valueB( x );
  }
}

void ChFun::set( size_t n, const double& startX, const double& endX )
{
  chebfun::set(n,startX,endX);
  m_errors.resize(n + 1);
  m_isFixed.resize( n + 1 );
  m_explicitlySet.resize( n + 1 );
}

void ChFun::reset(size_t n)
{
  set( n, startX(), endX() );
}

/** Sets a new value to the i-th parameter.
 *  @param i :: The parameter index
 *  @param value :: The new value
 *  @param explicitlySet :: A boolean falgging the parameter as explicitly set (by user)
 */
void ChFun::setParameter(size_t i, const double& value, bool explicitlySet)
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  setP( i, value );
  if (explicitlySet)
  {
    m_explicitlySet[i] = true;
  }
  this->updateStateRequired();
}

/** Sets a new parameter description to the i-th parameter.
 *  @param i :: The parameter index
 *  @param description :: New parameter description
 */
void ChFun::setParameterDescription(size_t i, const std::string& description)
{
  // description is fixed and cannot be changed
}

/** Get the i-th parameter.
 *  @param i :: The parameter index
 *  @return the value of the requested parameter
 */
double ChFun::getParameter(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  return ypoints()[i];
}

/**
 * Sets a new value to a parameter by name.
 * @param name :: The name of the parameter.
 * @param value :: The new value
 * @param explicitlySet :: A boolean flagging the parameter as explicitly set (by user)
 */
void ChFun::setParameter(const std::string& name, const double& value, bool explicitlySet)
{
  bool nameOK = name.size() > 1;
  size_t index = 0;
  if ( nameOK )
  {
    try
    {
      index = boost::lexical_cast<size_t>( name.substr(1) );
    }
    catch(...)
    {
      nameOK = false;
    }
  }
  if ( !nameOK )
  {
    std::ostringstream msg;
    msg << "ChFun parameter (" << name << ") does not exist.";
    throw std::invalid_argument(msg.str());
  }
  setParameter( index, value, explicitlySet );
}

/**
 * Sets a new description to a parameter by name.
 * @param name :: The name of the parameter.
 * @param description :: New parameter description
 */
void ChFun::setParameterDescription(const std::string& name, const std::string& description)
{
  throw std::runtime_error("Canot reset parameter description for ChFun.");
}


/**
 * Parameters by name.
 * @param name :: The name of the parameter.
 * @return the value of the named parameter
 */
double ChFun::getParameter(const std::string& name)const
{
  bool nameOK = name.size() > 1;
  size_t index = 0;
  if ( nameOK )
  {
    try
    {
      index = boost::lexical_cast<size_t>( name.substr(1) );
    }
    catch(...)
    {
      nameOK = false;
    }
  }
  if ( !nameOK )
  {
    std::ostringstream msg;
    msg << "ChFun parameter (" << name << ") does not exist.";
    throw std::invalid_argument(msg.str());
  }
  return getParameter( index );
}

/**
 * Returns the index of the parameter named name.
 * @param name :: The name of the parameter.
 * @return the index of the named parameter
 */
size_t ChFun::parameterIndex(const std::string& name)const
{
  bool nameOK = name.size() > 1;
  size_t index = 0;
  if ( nameOK )
  {
    try
    {
      index = boost::lexical_cast<size_t>( name.substr(1) );
    }
    catch(...)
    {
      nameOK = false;
    }
  }
  if ( !nameOK )
  {
    std::ostringstream msg;
    msg << "ChFun parameter (" << name << ") does not exist.";
    throw std::invalid_argument(msg.str());
  }
  return index;
}

/** Returns the name of parameter i
 * @param i :: The index of a parameter
 * @return the name of the parameter at the requested index
 */
std::string ChFun::parameterName(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  return "Y" + boost::lexical_cast<std::string>( i );
}

/** Returns the description of parameter i
 * @param i :: The index of a parameter
 * @return the description of the parameter at the requested index
 */
std::string ChFun::parameterDescription(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  return "Function value at point " + boost::lexical_cast<std::string>(i);
}

/** 
 * Get the fitting error for a parameter
 * @param i :: The index of a parameter
 * @return :: the error
 */
double ChFun::getError(size_t i) const
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  return m_errors[i];
}

/** 
 * Set the fitting error for a parameter
 * @param i :: The index of a parameter
 * @param err :: The error value to set
 */
void ChFun::setError(size_t i, double err)
{
  if (i >= nParams())
  {
    throw std::out_of_range("ChFun parameter index out of range.");
  }
  m_errors[i] = err;
}

/**
 * query if the parameter is fixed
 * @param i :: The index of a declared parameter
 * @return true if parameter i is active
 */
bool ChFun::isFixed(size_t i)const
{
  if (i >= nParams())
    throw std::out_of_range("ChFun parameter index out of range.");
  return m_isFixed[i];
}

/** This method doesn't create a tie
 * @param i :: A declared parameter index to be fixed
 */
void ChFun::fix(size_t i)
{
  if ( isFixed(i) ) return;
  m_isFixed[i] = true;
}

/** Makes a parameter active again. It doesn't change the parameter's tie.
 * @param i :: A declared parameter index to be restored to active
 */
void ChFun::unfix(size_t i)
{
  if ( !isFixed(i) ) return;
  m_isFixed[i] = false;
}

void ChFun::declareParameter(const std::string&, double, const std::string&)
{
  throw std::runtime_error("Cannot declare new parameter for ChFun.");
}

void ChFun::setAttribute(const std::string& attName,const IFunction::Attribute& att)
{
  IFunction1D::setAttribute( attName, att );
  if ( attName == "n" )
  {
    reset( att.asInt() );
  }
  else if ( attName == "StartX" )
  {
    set( n(), att.asDouble(), endX() );
  }
  else if ( attName == "EndX" )
  {
    set( n(), startX(), att.asDouble() );
  }
}

/// Checks if a parameter has been set explicitly
bool ChFun::isExplicitlySet(size_t i)const
{
  if (i >= nParams())
  {
    throw std::out_of_range("ParamFunction parameter index out of range.");
  }
  return m_explicitlySet[i];
}


} // Numeric
