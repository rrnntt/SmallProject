#ifndef KERNEL_PARSEDEXPRESSIONSTRING_H
#define KERNEL_PARSEDEXPRESSIONSTRING_H

#include "Kernel/DllExport.h"
#include "Kernel/EParser.h"

#include <iostream>

namespace Kernel
{

/**
 * A string with a EParser attached to it
 */
class KERNEL_EXPORT ParsedExpressionString
{
public:
  ParsedExpressionString(const std::string& str);
  void reset(const std::string& str);
  operator const std::string& () const {return m_expression;}
  const EParser& parser() const {return m_parser;}
  const EParser& parser(size_t i) const;
  size_t size() const {return m_expression.size();}
protected:
  const EParser& findParser(const EParser& p, size_t i) const;
  std::string m_expression;///< the string
  EParser m_parser; ///< the parser
};

inline std::ostream& operator<<(std::ostream& ostr, const ParsedExpressionString& ps)
{
  ostr << static_cast<std::string>(ps);
  return ostr;
}

} // KERNEL

#endif // KERNEL_PARSEDEXPRESSIONSTRING_H
