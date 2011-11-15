#include "Formula/ScriptLine.h"
#include "Formula/ScriptModule.h"
#include "Formula/Expression.h"
#include "Formula/Bool.h"

//#include <QFile>
#include <algorithm>
#include <stdexcept>

namespace Formula
{

  ScriptLineDef::ScriptLineDef(boost::shared_ptr<Namespace> ns,
                const std::string& type,
                const std::string& name,
                const std::string& value)
   //:m_type(type),m_name(name),m_value(value)
  {
    ns->addVariable(type,name,value);
  }

  void ScriptLineDef::eval()
  {
  }

  //-------------------------------------------------------------------------//

  void ScriptLineSet::eval()
  {
    if (!m_expression)
    {
      throw std::runtime_error("Expression failure");
    }
    //m_expression->logPrint();
    m_namespace->setVariable(m_var,m_expression->eval());
  }

//-------------------------------------------------------------------------//


  /**
    * @param name Function name only (without arguments)
    */
  ScriptLineCall::ScriptLineCall(boost::shared_ptr<Namespace> ns,
                                 ScriptFunctionGetter& getter,
                                 const std::string& name):
      m_namespace(ns),m_call_function(getter.getFunction(name))
  {
    if (!m_call_function)
    {
      throw std::invalid_argument("Unknown function "+name+" in module ");
    }
  }

  /**
    * @param line A script line to call a function, e.g. "calc(x=10)"
    */
  ScriptLineCall::ScriptLineCall(boost::shared_ptr<Namespace> ns,
                                 ScriptFunctionGetter& getter,
                                 const EParser& code):
      m_namespace(ns)
  {
    std::string name(code.name());
    m_call_function = getter.getFunction(name);
    if (!m_call_function)
    {
      throw std::invalid_argument("Unknown function "+name+" in module ");
    }
    for(int i=0;i<code.size();++i) // looping the function arguments
    {
      const EParser& arg = code[i];
      if (arg.name() != "=")
      {
        throw std::invalid_argument("Actual function arguments must be passed as name=value");
      }
      std::string name = arg[0]->name();
      const EParser& value = arg[1];
      setArgument(name,value);
    }
    check();
  }

  /**
    * @param argName Formal argument name
    * @param varName Name of the actual argument
    */
  void ScriptLineCall::setArgument(const std::string& argName,const std::string& varName)
  {
    Expression expr(m_namespace,varName);
    Variable_ptr var(expr.eval().clone());
    m_arguments[argName] = var;
  }

  void ScriptLineCall::setArgument(const std::string& argName,const EParser& varName)
  {
    Expression expr(m_namespace,varName);
    Variable_ptr var(expr.eval().clone());
    m_arguments[argName] = var;
  }

  void ScriptLineCall::check()
  {
    const std::vector<std::string>& args = m_call_function->arguments();
    for(int i=0;i<int(args.size());++i)
    {
      if (!m_arguments[args[i]])
      {
        throw std::invalid_argument("Not all actual arguments set");
      }
    }
  }

  void ScriptLineCall::eval()
  {
    std::map<std::string,Variable_ptr>::iterator it = m_arguments.begin();
    std::map<std::string,Variable_ptr>::iterator end = m_arguments.end();
    for(;it != end; ++it)
    {
      m_call_function->setArgument(it->first,*it->second);
    }
    m_call_function->eval();
  }

  //-------------------------------------------------------------------------//
  //-------------------------------------------------------------------------//

  ScriptLineBlock::ScriptLineBlock(boost::shared_ptr<Namespace> ns,const std::string& name)
  {
    Namespace* local_namespace = new Namespace(*ns,name);
    m_local_namespace = ns->addNamespace(local_namespace);
  }

  void ScriptLineBlock::eval()
  {
    for(int i=0;i<int(m_lines.size());++i)
    {
      m_lines[i]->eval();
    }
  }

  void ScriptLineBlock::addLines(const std::string& lines)
  {
    EParser code;
    code.parse(lines);
    addLines(code);
  }

  void ScriptLineBlock::addLines(const EParser& parser)
  {
    if (parser.name() == ";")
    {
      for(int i=0;i<parser.size();++i)
      {
        addLine(parser[i]);
      }
    }
    else
    {
      addLine(parser);
    }
  }

  /**
    * Add a line of script to the function:
    * Scalar: x = 1
    * Scalar: y = 2
    * Scalar: z
    * z = x + y
    */
  void ScriptLineBlock::addLine(const std::string& line)
  {
    EParser code;
    code.parse(line);
    addLine(code);
  }

  void ScriptLineBlock::addLine(const EParser& code)
  {
    ScriptLine* line = createLine(code);
    if (line)
    {
      m_lines.push_back(ScriptLine_ptr(line));
    }
    else
    {
      throw std::runtime_error("Cannot create script line: "+code.str());
    }
  }

  ScriptLine* ScriptLineBlock::createLine(const EParser& code)
  {
    if (code.name() == ":") // define variable
    {
      std::string type = code[0]->name();
      std::string name,value = "";
      if (code[1]->name() != "=")
      {
        name = code[1]->name();
      }
      else
      {
        name = code[1][0].name();
        Expression valExpr(m_local_namespace,(*code[1])[1]->str());
        value = valExpr.eval().toString();
      }
      return new ScriptLineDef(m_local_namespace,type,name,value);
    }
    else if (code.name() == "=") // assign new value
    {
      std::string name = code[0]->name();
      const EParser& value = code[1];
      return new ScriptLineSet(m_local_namespace,name,Expression_ptr(new Expression(m_local_namespace,value)));
    }
    else if (code.name() == "if") // if(cond1:expr1,cond2:(expr2;expr3;expr4;...),def_expr)
    {
      ScriptLineIf *sif = new ScriptLineIf(m_local_namespace);
      size_t n = code.size();
      for(size_t i=0;i<n;++i)
      {
        const EParser& clause = code[i];
        if (clause.name() == ":")
        {
          Expression* cond = new Expression(m_local_namespace,clause[0]);
          ScriptAnonymousBlock* block = new ScriptAnonymousBlock(this);
          block->addLines(clause[1]);
          sif->addCondition(Expression_ptr(cond),ScriptAnonymousBlock_ptr(block));
        }
        else
        {
          ScriptAnonymousBlock* block = new ScriptAnonymousBlock(this);
          block->addLines(clause);
          sif->addDefault(ScriptAnonymousBlock_ptr(block));
        }
      }
      return sif;
    }
    else if (code.name() == "for") // for(Scalar: i=0,i<10,i = i + 1,(line1;line2;line3;...))
    {
      if (code.size() != 4)
      {
        throw std::runtime_error("Script line -for- has a wrong structure");
      }
      ScriptAnonymousBlock_ptr block = ScriptAnonymousBlock_ptr(new ScriptAnonymousBlock(this));
      ScriptLine* init = block->createLine(code[0]);
      Expression_ptr end_cond(new Expression(block->getLocalNamespace_ptr(),code[1]));
      ScriptLine* next = block->createLine(code[2]);
      block->addLines(code[3]);
      ScriptLineFor* sfor = new ScriptLineFor(init,end_cond,next,block);
      return sfor;
    }
    else // function call
    {
      std::string fnName = code.name();
      ScriptLineCall* call = new ScriptLineCall(m_local_namespace,*this,fnName);
      for(int i=0;i<code.size();++i) // looping the function arguments
      {
        const EParser& arg = code[i];
        if (arg.name() != "=")
        {
          throw std::invalid_argument("Actual function arguments must be passed as name=value");
        }
        std::string name = arg[0]->name();
        const EParser& value = arg[1];
        call->setArgument(name,value);
      }
      call->check();
      return call;
    }
    return NULL;
  }

  //-------------------------------------------------------------------------//

  ScriptFunction::ScriptFunction(ScriptModule* sm,const std::string& fnName,const std::string& retType):
      ScriptLineBlock(sm->getNamespace(),fnName),m_module(sm)
  {
    if (!retType.empty())
    {
      m_local_namespace->addVariable(retType,"return");
    }
  }

  std::string ScriptFunction::name()const
  {
    return m_local_namespace->name();
  }

  void ScriptFunction::defineArgument(const std::string& type,const std::string& name, const std::string& value)
  {
    m_arguments.push_back(name);
    m_local_namespace->addVariable(type,name,value);
  }

  void ScriptFunction::setArgument(const std::string& name,const Variable& value)
  {
    std::vector<std::string>::iterator it = std::find(m_arguments.begin(),m_arguments.end(),name);
    if (it == m_arguments.end())
    {
      throw std::invalid_argument("Invalid function argument "+name+" in function "+m_local_namespace->name());
    }
    m_local_namespace->setVariable(name,value);
  }

  void ScriptFunction::setArgument(const std::string& name,const std::string& value)
  {
    std::vector<std::string>::iterator it = std::find(m_arguments.begin(),m_arguments.end(),name);
    if (it == m_arguments.end())
    {
      throw std::invalid_argument("Invalid function argument "+name+" in function "+m_local_namespace->name());
    }
    m_local_namespace->setVariable(name,value);
  }

  void ScriptFunction::eval()
  {
    for(int i=0;i<int(m_lines.size());++i)
    {
      m_lines[i]->eval();
    }
  }

  boost::shared_ptr<ScriptFunction> ScriptFunction::getFunction(const std::string& fnName)
  {
    return m_module->getFunction(fnName);
  }

//------------------------------------------------------------------------------------//

  void ScriptLineIf::eval()
  {
    Bool True(true);
    int j = -1;
    for(int i=0;i<m_conditions.size();++i)
    {
      if (True == m_conditions[i]->eval())
      {
        j = i;
        break;
      }
    }
    if (j >= 0)
    {
      m_blocks[j]->eval();
    }
    else if (m_default_block)
    {
      m_default_block->eval();
    }
  }

  void ScriptLineIf::addCondition(boost::shared_ptr<Expression> cond,ScriptAnonymousBlock_ptr block)
  {
    m_conditions.push_back(cond);
    m_blocks.push_back(block);
  }

  void ScriptLineIf::addDefault(ScriptAnonymousBlock_ptr block)
  {
    m_default_block = block;
  }
//------------------------------------------------------------------------------------------//

  ScriptLineFor::ScriptLineFor(ScriptLine* init,
                           boost::shared_ptr<Expression> end_cond,
                           ScriptLine* next,
                           ScriptAnonymousBlock_ptr block)
  {
    m_init = ScriptLine_ptr(init);
    m_end_condition = end_cond;
    m_next = ScriptLine_ptr(next);
    m_block = block;
  }


  void ScriptLineFor::eval()
  {
    m_init->eval();
    Bool True(true);
    while(True == m_end_condition->eval())
    {
      m_block->eval();
      m_next->eval();
    }
  }


} // Formula
