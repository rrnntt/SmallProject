#include "API/Column.h"
#include "API/NumericColumn.h"
#include "API/CompoundColumn.h"

#include <sstream>

namespace API
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

/// Is the data numeric (converts to double)?
bool Column::isNumeric() const
{
  return dynamic_cast<const NumericColumn*>(this) != NULL;
}

/// If this column is numeric returs a cast pointer to NumericColumn, therwise nullptr
NumericColumn* Column::asNumeric()
{
  return dynamic_cast<NumericColumn*>(this);
}

/// Is it a column of compound data (struct or class)
bool Column::isCompound() const
{
  return dynamic_cast<const CompoundColumn*>(this) != nullptr;
}

/// Returns a cast pointer to CompoundColumn or nullptr
CompoundColumn* Column::asCompound()
{
  return dynamic_cast<CompoundColumn*>(this);
}

} // API
