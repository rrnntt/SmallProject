#include "Formula/ScriptModule.h"
#include "Formula/ScriptLine.h"
#include "Formula/Parser.h"
#include "Formula/Namespace.h"
#include "Formula/Function.h"
#include "Formula/Expression.h"
#include "Formula/ScriptModuleFunction.h"

//#include <QFile>
#include <sstream>

namespace Formula
{

  ScriptModule::ScriptModule(boost::shared_ptr<Namespace> ns,const std::string& name):
      m_namespace(ns),m_name(name)
  {

  }

  void ScriptModule::addVariable(const std::string& type,const std::string& name,const std::string& value)
  {
    m_namespace->addVariable(type,name,value);
  }

  void ScriptModule::setVariable(const std::string& name,const Variable& v)
  {
    m_namespace->setVariable(name,v);
  }

  void ScriptModule::addFunction(const std::string& fnName, ScriptFunction* fun)
  {
    //FunMap::iterator it = m_functions.find(fnName);
    m_functions[fnName] = ScriptFunction_ptr(fun);
  }


  boost::shared_ptr<ScriptFunction> ScriptModule::getFunction(const std::string& fnName)
  {
    FunMap::iterator it = m_functions.find(fnName);
    if (it != m_functions.end())
    {
      return it->second;
    }
    return boost::shared_ptr<ScriptFunction>();
  }

  /**
    * @param funLine A function declaration line, eg: "Scalar: calc(Scalar: x)",
    *  or "doStuff(Bool: way)", or "onCreate"
    */
  boost::shared_ptr<ScriptFunction> ScriptModule::defFunction(const std::string& funLine)
  {
    Parser parser;
    parser.parse(funLine);

    // Get function name and return type
    const Parser& name_and_args = parser.name() != ":" ? parser : parser[1];
    std::string fnName = name_and_args.name();
    std::string retType;
    if (parser.name() == ":")
    {
      retType = parser[0].name();
    }

    // Create function
    ScriptFunction* fun(new ScriptFunction(this,fnName,retType));
    addFunction(fnName,fun);

    // Define arguments
    if (name_and_args.size() > 0)
    {
      for(int i=0;i<name_and_args.size();++i)
      {
        const Parser& arg = name_and_args[i];
        if (arg.name() == ":")
        {
          std::string type = arg[0].name();
          std::string name,value = "";
          if (arg[1].name() != "=")
          {
            name = arg[1].name();
          }
          else
          {
            name = arg[1][0].name();
            value = arg[1][1].name();
          }
          fun->defineArgument(type,name,value);
        }
      }
    }

    // Add the function to the namespace to be available in expressions
    if (!retType.empty())
    {
      m_namespace->addFunction(fnName,new ScriptModuleFunction(m_functions[fnName]));
    }
    return m_functions[fnName];
  }

  //void ScriptModule::load(const std::string& fName)
  //{
  //  QDomDocument doc("mydocument");
  //  QFile file(QString::fromStdString(fName));
  //  if (!file.open(QIODevice::ReadOnly))
  //      return;
  //  if (!doc.setContent(&file)) {
  //      file.close();
  //      return;
  //  }
  //  file.close();

  //  QDomElement delem = doc.documentElement();

  //  load(delem);

  //}

  //void ScriptModule::load(QDomElement elem)
  //{
  //  QString varsStr = elem.attribute("vars");
  //  if (!varsStr.isEmpty())
  //  {
  //    Parser vars;
  //    vars.parse(varsStr.toStdString());
  //    vars.toList();
  //    for(int i = 0;i < vars.size(); ++i)
  //    {
  //      const Parser& var = vars[i];
  //      if (var.name() != ":")
  //      {
  //        throw std::runtime_error("Syntax error in variable definition: "+var.str());
  //      }
  //      std::string type = var[0].name();
  //      std::string name;
  //      std::string value;
  //      if (var[1].name() == "=")
  //      {
  //        name = var[1][0].name();
  //        value = var[1][1].name();
  //      }
  //      else
  //      {
  //        name = var[1].name();
  //      }
  //      addVariable(type,name,value);
  //    }
  //  }
  //  QDomElement fun_elem = elem.firstChildElement("function");
  //  while(!fun_elem.isNull())
  //  {
  //    std::string fnName = fun_elem.attribute("def").toStdString();
  //    ScriptFunction_ptr fun = defFunction(fnName);
  //    QString text = fun_elem.text();
  //    fun->addLines(text.toStdString());
  //    fun_elem = fun_elem.nextSiblingElement("function");
  //  }
  //}

  /**
    * Run a function given its call string, eg "doSomething(1,2,opt=True)"
    */
  boost::shared_ptr<Variable> ScriptModule::runFunction(const std::string& fnStr)
  {
    Kernel::EParser pars;
    pars.parse(fnStr);
    std::string fnName = pars.name();
    ScriptFunction_ptr fun = getFunction(fnName);
    if (fun)
    {
      bool keyWords = false;
      const std::vector<std::string>& argNames = fun->arguments();
      for(int i=0;i<pars.size();++i)
      {
        const Kernel::EParser& arg = pars[i];
        if (!keyWords){ keyWords = (arg.name() == "=");}
        if (!keyWords)
        {
          if (i >= argNames.size())
          {
            throw std::invalid_argument("Wrong number of arguments in "+fnName);
          }
          Expression valExpr(m_namespace,arg);
          fun->setArgument(argNames[i],valExpr.eval());
        }
        else
        {
          if (arg.name() != "=")
          {
            throw std::invalid_argument("Keyword argument expected in "+fnName);
          }
          Expression valExpr(m_namespace,arg[1]);
          fun->setArgument(arg[0].name(),valExpr.eval());
        }
      }
      fun->eval();
      Variable_ptr res = fun->getLocalNamespace_ptr()->getVariable("return");
      return res;
    }
    return Variable_ptr();
  }

  void ScriptModule::logFuns(const std::string& pads)const
  {
    std::cerr << "Functions in module:\n";
    for(FunMap::const_iterator it = m_functions.begin(); it != m_functions.end(); ++it)
    {
      std::cerr << pads << it->first << std::endl;
    }
    std::cerr << std::endl;
  }

  /**
    * Define arguments for a function from a script string.
    * @param fun The function which arguemnts are being defined
    * @param args A script string defining the arguments: eg "Scalar: x",
    *    "Scalar: x, Scalar: y = 2"
    */
  void ScriptModule::defArguments(ScriptFunction* fun,const std::string& args)
  {
    Parser parser;
    parser.parse(args);
    parser.toList();
    for(int i = 0; i < parser.size(); ++i)
    {
      const Parser& arg = parser[i];
      if (arg.name() != ":")
      {
        throw std::runtime_error("Syntax error in variable definition: "+arg.str());
      }
      std::string type = arg[0].name();
      std::string name;
      std::string value;
      if (arg[1].name() == "=")
      {
        name = arg[1][0].name();
        Expression expr(m_namespace,arg[1][1].str());
        value = expr.eval().toString();
      }
      else
      {
        name = arg[1].name();
      }
      fun->defineArgument(type,name,value);
    }
  }

//  void ScriptModule::eval(const std::string& fName)
//  {
//  }

} // Formula
