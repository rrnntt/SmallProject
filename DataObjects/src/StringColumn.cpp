#include "DataObjects/StringColumn.h"

namespace DataObjects
{

  DECLARE_COLUMN(StringColumn,string);

//  void StringColumn::read(std::istream& s, int index)
//  {
//    std::string& str = m_data[index];
//    s >> str;
//    if (str.empty()) return;
//    if (str[0] == '"')
//    {
//      if (str == "\"\"") 
//      {
//        str.clear();
//        return;
//      }
//      str.erase(0,1);
//      char c;
//      while(s.good())
//      {
//        s.get(c);
//        if (c == '"' && (str.empty() || str.back() != '\\')) return;
//        str.push_back(c);
//      }
//    }
//  }
//
///// Save into a file
//void StringColumn::saveAscii(std::ostream& s, int index) const
//{
//  s << '"';
//  this->print(s,index);
//  s << '"';
//}
//
///// Read from a file
//void StringColumn::loadAscii(std::istream& s, int index)
//{
//  this->read(s,index);
//}

std::string StringColumn::asString(size_t i) const
{
  return m_data[i];
}

void StringColumn::fromString(const std::string& str,size_t i)
{
  m_data[i] = str;
}

}  // DataObjects

