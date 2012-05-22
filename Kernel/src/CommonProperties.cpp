#include "Kernel/CommonProperties.h"

#include <stdexcept>
#include <algorithm>

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

  StringProperty::StringProperty(const std::vector<std::string>& values, const std::string& defValue):
  m_values(values)
  {
    if ( !values.empty() )
    {
      if ( !defValue.empty() )
      {
        auto it = std::find(values.begin(), values.end(), defValue);
        if ( it != values.end() )
        {
          m_value = defValue;
        }
        else
        {
          m_value = values[0];
        }
      }
      else
      {
        m_value = values[0];
      }
    }
    else
    {
      m_value = defValue;
    }
  }

  StringProperty::operator std::string() const
  {
    return m_value;
  }

  Property& StringProperty::operator=(const std::string& str)
  {
    if ( !m_values.empty() )
    {
      auto it = std::find(m_values.begin(), m_values.end(), str);
      if ( it != m_values.end() )
      {
        m_value = str;
      }
      else
      {
        throw std::invalid_argument("String property value " + str + " is invalid.");
      }
    }
    else
    {
      m_value = str;
    }
    return *this;
  }

} // namespace Kernel
