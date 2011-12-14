#include "DataObjects/StringColumn.h"

namespace DataObjects
{

  DECLARE_COLUMN(StringColumn,string);

  void StringColumn::read(std::istream& s, int index)
  {
    std::string& str = m_data[index];
    s >> str;
    if (str.empty()) return;
    if (str[0] == '"')
    {
      char c;
      while(s.good())
      {
        s.get(c);
        if (c == '"' && str.back() != '\\') return;
        str.push_back(c);
      }
    }
  }

/// Save into a file
void StringColumn::saveAsci(std::ostream& s, int index) const
{
  s << '"';
  this->print(s,index);
  s << '"';
}

/// Read from a file
void StringColumn::loadAsci(std::istream& s, int index)
{
  this->read(s,index);
}

}  // DataObjects

