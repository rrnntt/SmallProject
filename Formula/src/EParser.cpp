#include "Formula/EParser.h"

#include <boost/spirit/include/qi.hpp>
#include <stdexcept>

namespace Formula
{

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
