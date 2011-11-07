#include "Formula/EParser.h"

#include <boost/spirit/include/qi.hpp>
#include <stdexcept>

namespace Formula
{

std::string::const_iterator IParser::match(std::string::const_iterator start,std::string::const_iterator end)
{
  if (start == end)
  {
    m_start = m_end = end;
    return m_end;
  }
  m_start = start;
  m_end = this->test(start,end);

  return m_start;
}

std::string::const_iterator CharParser::test(std::string::const_iterator start,std::string::const_iterator end) const
{
  if (m_chars.empty())
  {// match any char if string is not empty
    return ++start;
  }
  else
  {// match specific char (one of the m_chars)
    auto found = m_chars.find(*start);
    if ( found == std::string::npos )
    {
      return start;
    }
    else
    {
      return ++start;
    }
  }
  return start;
}

/*--- EParser ---*/  
EParser::EParser()
{

  m_operators.reset(new Operators());
  // Define binary operators. Put them in the reverse precedence order (from lower to higher prec.)
  std::vector<std::string> ops;
  ops.push_back(";");
  ops.push_back(",");
  ops.push_back("= != > < <= >=");
  ops.push_back("&& || ^^");
  ops.push_back("+ -");
  ops.push_back("* /");
  ops.push_back("^");

  m_operators->add_operators(ops);

  // Define unary operators
  std::set<std::string> unary;
  unary.insert("+");
  unary.insert("-");

  m_operators->add_unary(unary);
}

void EParser::parse(const std::string& str)
{
}

} // Formula
