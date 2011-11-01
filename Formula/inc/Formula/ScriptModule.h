#ifndef FORMULA_SCRIPTMODULE_H_
#define FORMULA_SCRIPTMODULE_H_

#include "Formula/DllExport.h"
#include "Formula/MemberFunction.h"
#include "Formula/ScriptModuleFunction.h"
#include "Formula/Parser.h"

//#include <QtXml/QDomElement>
#include <boost/shared_ptr.hpp>
#include <map>

namespace Formula
{

  class Namespace;
  class Variable;
  class ScriptFunction;

  /**
    * ScriptModule is a collection of ScriptFunctions.
    */
  class FORMULA_EXPORT ScriptModule
  {
  public:
    ScriptModule(boost::shared_ptr<Namespace> ns,const std::string& name);
    std::string name()const{return m_name;}
    //void load(QDomElement elem);
    //void load(const std::string& fName);
    //void eval(const std::string& fName);
    void addVariable(const std::string& type,const std::string& name,const std::string& value);
    void setVariable(const std::string& name,const Variable& v);
    boost::shared_ptr<Namespace> getNamespace(){return m_namespace;}
    boost::shared_ptr<ScriptFunction> getFunction(const std::string& fnName);
    boost::shared_ptr<ScriptFunction> defFunction(const std::string& fnName);
    boost::shared_ptr<Variable> runFunction(const std::string& fnStr);
    void logFuns(const std::string& pads = "")const;
    template<class C>
    void addMemberFunction(C* aClass,
                           void (C::*method)(std::vector<Variable_ptr>&,boost::shared_ptr<Variable>),
                           const std::string& fnName,
                           const std::string& args = "",
                           const std::string& retType = "");
  protected:
    void addFunction(const std::string& fnName, ScriptFunction* fun);
    void defArguments(ScriptFunction* fun,const std::string& args);
    boost::shared_ptr<Namespace> m_namespace;
    std::string m_name;
    typedef std::map< std::string,boost::shared_ptr<ScriptFunction> > FunMap;
    FunMap m_functions;
  };

  typedef boost::shared_ptr<ScriptModule> ScriptModule_ptr;

  template<class C>
  void ScriptModule::addMemberFunction(C* aClass,
                         void (C::*method)(std::vector<Variable_ptr>&,boost::shared_ptr<Variable>),
                         const std::string& fnName,
                         const std::string& args,
                         const std::string& retType)
  {
    MemberFunction<C> *mf = new MemberFunction<C>(aClass,method,this,fnName,retType);
    this->addFunction(fnName,mf);
    if (!args.empty())
    {
      this->defArguments(mf,args);
    }
    if (!retType.empty())
    {
      m_namespace->addFunction(fnName,new ScriptModuleFunction(m_functions[fnName]));
    }
  }

} // Formula

#endif // FORMULA_SCRIPTMODULE_H_
