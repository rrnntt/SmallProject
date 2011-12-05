#include "DataObjects/Column.h"

#include <sstream>

namespace DataObjects
{

template<>
bool Column::isType<bool>()const
{
    return isBool();
}

std::ostream& operator<<(std::ostream& s,const Boolean& b)
{
    s << (b.value?"true":"false");
    return s;
}

std::string Column::asString(size_t i) const
{
  std::stringstream ostr;
  this->print(ostr,i);
  return ostr.str();
}

} // DataObjects
