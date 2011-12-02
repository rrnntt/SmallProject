#include "Teach/Problem.h"

namespace Teach
{

  Problem::Problem():
  m_namespace(new Formula::Namespace)
  {
  }

  void Problem::read(std::istream& is)
  {
    std::string str;
    while( std::getline(is,str) )
    {
      if (str == "Text:")
      {
        m_text.read(is);
      }

      if (str == "Eqs:")
      {
        read_equations(is);
      }
    }
  }

  void Problem::read_equations(std::istream& is)
  {
    std::string str;
    while( std::getline(is,str) )
    {
      if (str.empty())
      {
        return;
      }
      Kernel::EParser parser;
      parser.parse(str);
      std::set<std::string> vars = parser.getVariables();
      for(auto v = vars.begin(); v != vars.end(); ++v)
      {
        m_namespace->addVariable("Scalar",*v);
      }

      Formula::Expression_ptr expr(new Formula::Expression(m_namespace,parser));
      m_equations.push_back(expr);
    }
  }

  void Problem::print(std::ostream& os)const
  {
    m_text.print(os);
    os << std::endl;
    if (!m_equations.empty())
    {
      os << "Equations:" << std::endl;
      std::for_each(m_equations.begin(),m_equations.end(),[&os](const Formula::Expression_ptr& e){
        os << e->str() << std::endl;
      });
      os << std::endl;
    }
  }

} // Problem

std::ostream& operator<<(std::ostream& os,const Teach::Problem& problem)
{
  problem.print(os);
  return os;
}
