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

std::istream& operator>>(std::istream& s,Boolean& b)
{
  std::string str;
  s >> str;
  b.value = (str == "true" || str == "1");
  return s;
}

std::string Column::asString(size_t i) const
{
  std::stringstream ostr;
  this->print(ostr,i);
  return ostr.str();
}

void Column::fromString(const std::string& str,size_t i) 
{
  std::istringstream istr(str);
  this->read(istr,i);
}

/// Save into a file
void Column::saveAsci(std::ostream& s, int index) const
{
  this->print(s,index);
}

/// Read from a file
void Column::loadAsci(std::istream& s, int index)
{
  this->read(s,index);
}

} // DataObjects
