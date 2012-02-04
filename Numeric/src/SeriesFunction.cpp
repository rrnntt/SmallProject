#include "Numeric/SeriesFunction.h"

#include <boost/lexical_cast.hpp>

namespace Numeric
{

SeriesFunction::SeriesFunction()
{
}

double SeriesFunction::getParameter(size_t i)const
{
  validateParameterIndex(i);
  return m_parameters[i];
}

/// Get parameter by name.
double SeriesFunction::getParameter(const std::string& pname)const
{
  try
  {
    size_t i = boost::lexical_cast<size_t>(pname.substr(1));
    return getParameter(i);
  }
  catch(...)
  {
    throw std::invalid_argument("Cannot find parameter "+pname+" in function "+name());
  }
}

/// Set i-th parameter
void SeriesFunction::setParameter(size_t i, const double& value)
{
  validateParameterIndex(i);
  m_parameters[i] = value;
}

/// Set parameter by name.
void SeriesFunction::setParameter(const std::string& pname, const double& value)
{
  try
  {
    size_t i = boost::lexical_cast<size_t>(pname.substr(1));
    setParameter(i,value);
  }
  catch(...)
  {
    throw std::invalid_argument("Cannot find parameter "+pname+" in function "+name());
  }
}

/// Total number of parameters
size_t SeriesFunction::nParams()const
{
  return m_parameters.size();
}

/// Returns the index of parameter name
size_t SeriesFunction::parameterIndex(const std::string& pname)const
{
  try
  {
    size_t i = boost::lexical_cast<size_t>(pname.substr(1));
    validateParameterIndex(i);
    return i;
  }
  catch(...)
  {
    throw std::invalid_argument("Cannot find parameter "+pname+" in function "+name());
  }
}

/// Returns the name of parameter i
std::string SeriesFunction::parameterName(size_t i)const
{
  validateParameterIndex(i);
  return "P"+boost::lexical_cast<std::string>(i);
}

/// Number of active (in terms of fitting) parameters.
/// Always equal to nParams()
size_t SeriesFunction::nActive()const
{
  return nParams();
}

/// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
double SeriesFunction::activeParameter(size_t i)const
{
  return getParameter(i);
}

/// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
void SeriesFunction::setActiveParameter(size_t i, double value)
{
  setParameter(i,value);
}

/// Returns the name of active parameter i
std::string SeriesFunction::nameOfActive(size_t i)const
{
  return parameterName(i);
}

/// Check if declared parameter i is fixed.
/// Always returns false.
bool SeriesFunction::isFixed(size_t i)const
{
  return false;
}

/// Fixes declared parameter i
void SeriesFunction::fix(size_t)
{
  // do nothing
}

/// Restores declared parameter i for fitting
void SeriesFunction::unfix(size_t)
{
  // do nothing
}

/// Returns a list of attribute names
std::vector<std::string> SeriesFunction::getAttributeNames()const
{
  return std::vector<std::string>(1,"Order");
}

/// Return a value of attribute attName
SeriesFunction::Attribute SeriesFunction::getAttribute(const std::string& attName)const
{
  return SeriesFunction::Attribute(m_order);
}

/// Set a value to attribute attName
void SeriesFunction::setAttribute(const std::string& attName,const SeriesFunction::Attribute& att)
{
  m_order = att.asInt();
}

/// Check if attribute attName exists
bool SeriesFunction::hasAttribute(const std::string& attName)const
{
  return attName == "Order";
}


} // namespace Numeric
