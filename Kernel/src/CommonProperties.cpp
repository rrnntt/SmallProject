#include "Kernel/CommonProperties.h"

#include <stdexcept>

namespace Kernel
{

  DoubleProperty::DoubleProperty(const Property& p)
  {
    const DoubleProperty* q = dynamic_cast<const DoubleProperty*>(&p);
    if (!q)
    {
      throw std::runtime_error("Property does not convert to DoubleProperty");
    }
    m_value = static_cast<double>(*q);
  }

  DoubleProperty::operator std::string() const
  {
    std::ostringstream ostr;
    ostr << m_value;
    return ostr.str();
  }

  Property& DoubleProperty::operator=(const std::string& str)
  {
    std::istringstream istr(str);
    istr >> m_value;
    return *this;
  }

  IntProperty::operator std::string() const
  {
    std::ostringstream ostr;
    ostr << m_value;
    return ostr.str();
  }

  Property& IntProperty::operator=(const std::string& str)
  {
    std::istringstream istr(str);
    istr >> m_value;
    return *this;
  }

  BoolProperty::operator std::string() const
  {
    std::ostringstream ostr;
    ostr << m_value;
    return ostr.str();
  }

  Property& BoolProperty::operator=(const std::string& str)
  {
    std::istringstream istr(str);
    istr >> m_value;
    return *this;
  }

  StringProperty::operator std::string() const
  {
    return m_value;
  }

  Property& StringProperty::operator=(const std::string& str)
  {
    m_value = str;
    return *this;
  }


} // namespace Kernel
