#include "Numeric/FunctionProperty.h"

#include <stdexcept>

namespace Numeric
{

  FunctionProperty::FunctionProperty(const Kernel::Property& prop)
  {
    auto p = dynamic_cast<const FunctionProperty*>(&prop);
    if (!p)
    {
      throw std::runtime_error("Property does not convert to FunctionProperty");
    }
    m_value = static_cast<IFunction_ptr>(*p);
  }

  Kernel::Property& FunctionProperty::operator=(const std::string& str)
  {
    throw std::runtime_error("FunctionProperty::operator=(const std::string& str) not implemented.");
  }

  FunctionProperty::operator std::string() const
  {
    throw std::runtime_error("FunctionProperty::operator  std::string() not implemented.");
  }

} // namespace Numeric
