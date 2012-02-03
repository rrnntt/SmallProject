#include "Numeric/SeriesFunction.h"

namespace Numeric
{

SeriesFunction::SeriesFunction()
{
}

double SeriesFunction::getParameter(size_t i)const
{
}

/// Get parameter by name.
double SeriesFunction::getParameter(const std::string& name)const
{
}

/// Set i-th parameter
void SeriesFunction::setParameter(size_t, const double& value)
{
}

/// Set parameter by name.
void SeriesFunction::setParameter(const std::string& name, const double& value)
{
}

/// Total number of parameters
size_t SeriesFunction::nParams()const
{
}

/// Returns the index of parameter name
size_t SeriesFunction::parameterIndex(const std::string& name)const
{
}

/// Returns the name of parameter i
std::string SeriesFunction::parameterName(size_t i)const
{
}

/// Number of active (in terms of fitting) parameters
size_t SeriesFunction::nActive()const
{
}

/// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
double SeriesFunction::activeParameter(size_t i)const
{
}

/// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
void SeriesFunction::setActiveParameter(size_t i, double value)
{
}

/// Returns the name of active parameter i
std::string SeriesFunction::nameOfActive(size_t i)const
{
}

/// Check if declared parameter i is fixed
bool SeriesFunction::isFixed(size_t i)const
{
}

/// Fixes declared parameter i
void SeriesFunction::fix(size_t i)
{
}

/// Restores declared parameter i for fitting
void SeriesFunction::unfix(size_t i)
{
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
