#include "Kernel/ParsedExpressionString.h"

#include <stdexcept>

namespace Kernel
{

/**
 * Constructor.
 */
ParsedExpressionString::ParsedExpressionString(const std::string& str):
m_expression(str)
{
  m_parser.parse(m_expression);
}

/**
 * Set a new expression string
 */
void ParsedExpressionString::reset(const std::string& str)
{
  m_expression = str;
  m_parser.parse(m_expression);
}

/**
 * Return the inner-most parser containing symbol at position i.
 * @param i :: Position of the symbol in the string
 */
const EParser& ParsedExpressionString::parser(size_t i) const
{
  if (i >= m_expression.size())
  {
    throw std::range_error("Index out of range");
  }
  return findParser(m_parser,i);
}

const EParser& ParsedExpressionString::findParser(const EParser& p, size_t index) const
{
  if (index < p.getStartPos() || index >= p.getStartPos() + p.getStringSize())
  {
    throw std::runtime_error("Index is outside parser's string");
  }
  if (p.size() == 0)
  {
    return p;
  }
  for(size_t k = 0; k < p.size(); ++k)
  {
    const EParser& pp = p[k];
    if (index >= pp.getStartPos() && index < pp.getStartPos() + pp.getStringSize())
    {
      return findParser(pp,index);
    }
  }
  return p;
}

} // Kernel
