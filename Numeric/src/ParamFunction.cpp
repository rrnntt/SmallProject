#include "Numeric/ParamFunction.h"

#include <algorithm>

namespace Numeric
{

ParamFunction::ParamFunction()
{
}

/// Get i-th parameter
double ParamFunction::getParameter(size_t i)const
{
  validateParameterIndex(i);
  return m_parameters[i].value;
}

/// Get parameter by name.
double ParamFunction::getParameter(const std::string& pname)const
{
  auto it = std::find_if(m_parameters.begin(),m_parameters.end(),[&pname](const ParamStorageType& s)->bool{
    return s.name == pname;
  });
  if (it == m_parameters.end()) throw std::runtime_error("Parameter " + pname + " not found in function " + this->name());
  return it->value;
}

/// Set i-th parameter
void ParamFunction::setParameter(size_t i, const double& value)
{
  validateParameterIndex(i);
  m_parameters[i].value = value;
}

/// Set parameter by name.
void ParamFunction::setParameter(const std::string& pname, const double& value)
{
  auto it = std::find_if(m_parameters.begin(),m_parameters.end(),[&pname](const ParamStorageType& s)->bool{
    return s.name == pname;
  });
  if (it == m_parameters.end()) throw std::runtime_error("Parameter " + pname + " not found in function " + this->name());
  it->value = value;
}

/// Total number of parameters
size_t ParamFunction::nParams()const
{
  return m_parameters.size();
}

/// Returns the index of parameter name
size_t ParamFunction::parameterIndex(const std::string& pname)const
{
  auto it = std::find_if(m_parameters.begin(),m_parameters.end(),[&pname](const ParamStorageType& s)->bool{
    return s.name == pname;
  });
  if (it == m_parameters.end()) throw std::runtime_error("Parameter " + pname + " not found in function " + this->name());
  return static_cast<size_t>(it - m_parameters.begin());
}

/// Returns the name of parameter i
std::string ParamFunction::parameterName(size_t i)const
{
  validateParameterIndex(i);
  return m_parameters[i].name;
}

/// Number of active (in terms of fitting) parameters
size_t ParamFunction::nActive()const
{
  return m_activeIndex.size();
}

/// Value of i-th active parameter. Override this method to make fitted parameters different from the declared
double ParamFunction::activeParameter(size_t i)const
{
  validateActiveParameterIndex(i);
  return m_parameters[m_activeIndex[i]].value;
}

/// Set new value of i-th active parameter. Override this method to make fitted parameters different from the declared
void ParamFunction::setActiveParameter(size_t i, double value)
{
  validateActiveParameterIndex(i);
  m_parameters[m_activeIndex[i]].value = value;
}

/// Returns the name of active parameter i
std::string ParamFunction::nameOfActive(size_t i)const
{
  validateActiveParameterIndex(i);
  return m_parameters[m_activeIndex[i]].name;
}

/// Check if declared parameter i is fixed
bool ParamFunction::isFixed(size_t i)const
{
  return std::find(m_activeIndex.begin(),m_activeIndex.end(),i) == m_activeIndex.end();
}

/// Fixes declared parameter i
void ParamFunction::fix(size_t i)
{
  auto it = std::find(m_activeIndex.begin(),m_activeIndex.end(),i);
  if (it != m_activeIndex.end())
  {
    m_activeIndex.erase(it);
  }
}

/// Restores declared parameter i for fitting
void ParamFunction::unfix(size_t i)
{
  auto it = std::find(m_activeIndex.begin(),m_activeIndex.end(),i);
  if (it == m_activeIndex.end())
  {
    it = std::find_if(m_activeIndex.begin(),m_activeIndex.end(),[&i](size_t j)->bool{return j > i;});
    m_activeIndex.insert(it,i);
  }
}

/**
 * Declare a new parameter.
 * @param pname :: A parameter name.
 * @param initValue :: Optional initial value.
 */
void ParamFunction::declareParameter(const std::string& pname, double initValue) 
{
  if (hasParameter(pname))
  {
    throw std::runtime_error("Parameter " + pname + " already defined.");
  }
  m_parameters.push_back(ParamStorageType());
  m_parameters.back().name = pname;
  m_parameters.back().value = initValue;
  m_activeIndex.resize(m_parameters.size());
  size_t i = 0;
  std::generate(m_activeIndex.begin(),m_activeIndex.end(),[&i]()->size_t{
    return i++;
  });
}

/**
 * Check if a parameter name was already declared.
 * @param pname :: A parameter name.
 */
bool ParamFunction::hasParameter(const std::string& pname) const
{
  return std::find_if(m_parameters.begin(),m_parameters.end(),[&pname](const ParamStorageType& s)->bool{
    return s.name == pname;
  }) != m_parameters.end();
}

/**
 * Check if a parameter index is valid. If not - throw range_error.
 * @param i :: An index to check.
 */
void ParamFunction::validateParameterIndex(size_t i) const
{
  if (i >= nParams()) throw std::range_error("Parameter index out of range");
}

/**
 * Check if an active parameter index is valid. If not - throw range_error.
 * @param i :: An index to check.
 */
void ParamFunction::validateActiveParameterIndex(size_t i) const
{
  if (i >= nActive()) throw std::range_error("Active parameter index out of range");
}

} // namespace Numeric
