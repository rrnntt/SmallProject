#ifndef FORMULA_SCRIPTLINE_H_
#define FORMULA_SCRIPTLINE_H_

#include "Formula/DllExport.h"
#include "Formula/Variable.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>

namespace Kernel
{
class EParser;
}

namespace Formula
{

class Namespace;
class Expression;
class ScriptModule;
class ScriptLineBlock;
class ScriptFunction;
class SyntaxParser;

/**
 * A base class for a script line.
 */
class FORMULA_EXPORT ScriptLine
{
public:
  virtual void eval() = 0;
};

/// Shared pointer to a script line
typedef boost::shared_ptr<ScriptLine> ScriptLine_ptr;

/// Variable definition script line
class FORMULA_EXPORT ScriptLineDef: public ScriptLine
{
public:
  ScriptLineDef(boost::shared_ptr<Namespace> ns,
                const std::string& type,
                const std::string& name,
                const std::string& value = "");
  void eval();
};

/// Evaluate an expression and assign the returned value to a variable
class FORMULA_EXPORT ScriptLineSet: public ScriptLine
{
public:
  ScriptLineSet(boost::shared_ptr<Namespace> ns,const std::string& name,boost::shared_ptr<Expression> expression):
      m_namespace(ns),m_var(name),m_expression(expression){}
  void eval();
private:
  boost::shared_ptr<Namespace> m_namespace;
  std::string m_var;
  boost::shared_ptr<Expression> m_expression;
};

/// Evaluate an expression
class FORMULA_EXPORT ScriptLineExpression: public ScriptLine
{
public:
  ScriptLineExpression(boost::shared_ptr<Namespace> ns, boost::shared_ptr<Expression> expression):
      m_namespace(ns),m_expression(expression){}
  void eval();
private:
  boost::shared_ptr<Namespace> m_namespace;
  boost::shared_ptr<Expression> m_expression;
};

/// Interface to a class that can return a pointer to a script function
class FORMULA_EXPORT ScriptFunctionGetter
{
public:
  virtual boost::shared_ptr<ScriptFunction> getFunction(const std::string& fnName) = 0;
};

/// Function call script line
class FORMULA_EXPORT ScriptLineCall: public ScriptLine
{
public:
  ScriptLineCall(boost::shared_ptr<Namespace> ns,ScriptFunctionGetter& getter,const std::string& line);
  ScriptLineCall(boost::shared_ptr<Namespace> ns,ScriptFunctionGetter& getter,const Kernel::EParser& code);
  void setArgument(const std::string& argName,const std::string& varName);
  void setArgument(const std::string& argName,const Kernel::EParser& varName);
  void eval();
  void check();
private:
  boost::shared_ptr<Namespace> m_namespace;
  boost::shared_ptr<ScriptFunction> m_call_function;
  typedef std::map<std::string,Variable_ptr> ArgMap;
  ArgMap m_arguments;
};

/**
 * A block of script lines. 
 */
class FORMULA_EXPORT ScriptLineBlock:public ScriptLine, public ScriptFunctionGetter
{
public:
  ScriptLineBlock();
  ScriptLineBlock(boost::shared_ptr<Namespace> ns,const std::string& name = "");
  void eval();
  /// Returns an empty pointer. Must be subclassed to work with functions
  boost::shared_ptr<ScriptFunction> getFunction(const std::string& fnName){return boost::shared_ptr<ScriptFunction>();}

  void addScript(const std::string& script);

  Namespace& getLocalNamespace(){return *m_local_namespace;}
  boost::shared_ptr<Namespace> getLocalNamespace_ptr(){return m_local_namespace;}
protected:
  ScriptLine* createLine(SyntaxParser* parser);
  ScriptLine* createLine(const Kernel::EParser& parser);
  boost::shared_ptr<Namespace> m_local_namespace;
  std::vector<ScriptLine_ptr> m_lines;
};

/// script block pointer
typedef boost::shared_ptr<ScriptLineBlock> ScriptLineBlock_ptr;

/**
 * An anonymous script block. It can only be created inside a script function as
 * the constructor takes an ScriptLineBlock pointer.
 */
class FORMULA_EXPORT ScriptAnonymousBlock: public ScriptLineBlock
{
public:
  ScriptAnonymousBlock(ScriptLineBlock* block):
      ScriptLineBlock(block->getLocalNamespace_ptr()),m_parent(block){}
  boost::shared_ptr<ScriptFunction> getFunction(const std::string& fnName){return m_parent->getFunction(fnName);}
protected:
  ScriptLineBlock* m_parent;
};

/// Pointer to an anonymous script block
typedef boost::shared_ptr<ScriptAnonymousBlock> ScriptAnonymousBlock_ptr;

/// Script function
class FORMULA_EXPORT ScriptFunction: public ScriptLineBlock
{
public:
  ScriptFunction(ScriptModule* sm,const std::string& fnName,const std::string& retType = "");
  void eval();
  void defineArgument(const std::string& type,const std::string& name, const std::string& value = "");
  void setArgument(const std::string& name,const Variable& value);
  void setArgument(const std::string& name,const std::string& value);
  size_t nArguments()const{return m_arguments.size();}
  const std::vector<std::string>& arguments()const{return m_arguments;}
  std::string name()const;
  ScriptModule& getModule(){return *m_module;}
  boost::shared_ptr<ScriptFunction> getFunction(const std::string& fnName);
protected:
  ScriptModule* m_module;
  std::vector<std::string> m_arguments;
};

typedef boost::shared_ptr<ScriptFunction> ScriptFunction_ptr;

/// An if statement
class FORMULA_EXPORT ScriptLineIf: public ScriptLine
{
public:
  ScriptLineIf(boost::shared_ptr<Namespace> ns):m_namespace(ns){}
  void eval();
  void addCondition(boost::shared_ptr<Expression> cond, ScriptLine_ptr line);
  void addDefault(ScriptLine_ptr block);
private:
  boost::shared_ptr<Namespace> m_namespace;
  std::vector<boost::shared_ptr<Expression> > m_conditions;
  std::vector<ScriptLine_ptr> m_blocks;
  ScriptLine_ptr m_default_block;
};

/// A for-loop
class FORMULA_EXPORT ScriptLineFor: public ScriptLine
{
public:
  ScriptLineFor(ScriptLine* init,
            boost::shared_ptr<Expression> end_cond,
            ScriptLine* next,
            ScriptLine_ptr block);
  void eval();
private:
  ScriptLine_ptr m_init;
  boost::shared_ptr<Expression> m_end_condition;
  ScriptLine_ptr m_next;
  ScriptLine_ptr m_block;
};

} // Formula

#endif // FORMULA_SCRIPTLINE_H_
