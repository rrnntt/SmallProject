#ifndef FORMULA_MEMBERFUNCTION_H
#define FORMULA_MEMBERFUNCTION_H

#include "Formula/ScriptLine.h"
#include "Formula/Namespace.h"

namespace Formula
{

  template<class C>
  class MemberFunction: public ScriptFunction
  {
  public:
    MemberFunction(
        C* aClass,
        void (C::*method)(std::vector<Variable_ptr>&,Variable_ptr),
        ScriptModule* sm,const std::string& fnName,const std::string& retType = ""):
        ScriptFunction(sm,fnName,retType),
        m_class(aClass),m_method(method)
    {

    }
    void eval()
    {
      Variable_ptr ret = m_local_namespace->getVariable("return");
      std::vector<Variable_ptr> args;
      std::vector<std::string>::iterator it = m_arguments.begin();
      for(;it != m_arguments.end();++it)
      {
        args.push_back(m_local_namespace->getVariable(*it));
      }
      (m_class->*m_method)(args,ret);
    }
  private:
    C* m_class;
    void (C::*m_method)(std::vector<Variable_ptr>&,Variable_ptr);
  };


} // Formula

#endif // FORMULA_MEMBERFUNCTION_H
