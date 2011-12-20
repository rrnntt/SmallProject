#include "Kernel/Property.h"

#include <stdexcept>

namespace Kernel
{

  Property& Property::operator=(const Property& p)
  {
    *this = static_cast<std::string>(p);
    return *this;
  }

} // namespace Kernel

std::ostream& operator << (std::ostream& ostr, const Kernel::Property& p)
{
  ostr << static_cast<const std::string>(p);
  return ostr;
}
