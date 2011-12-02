#ifndef PROBLEM_PROBLEM_H
#define PROBLEM_PROBLEM_H

#include "Teach/DllExport.h"
#include "Teach/Text.h"
#include "Formula/Expression.h"

#include <vector>
#include <istream>
#include <ostream>

namespace Teach
{

class TEACH_EXPORT Problem
{
public:
  Problem();
  void read(std::istream& is);
  void print(std::ostream& os)const;
protected:
  void read_equations(std::istream& is);
  Text m_text;
  std::vector<Formula::Expression_ptr> m_equations;
  Formula::Namespace_ptr m_namespace;
};

} // Problem

TEACH_EXPORT std::ostream& operator<<(std::ostream& os,const Teach::Problem& problem);


#endif // PROBLEM_PROBLEM_H
