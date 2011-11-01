#include "Expression.h"
#include "Function.h"
#include "TextObject.h"
#include "Str.h"
#include "Operators.h"

#include <iostream>
#include <locale>
#include <algorithm>
#include <map>
#include <sstream>

const size_t Expression::s_max_precedence = 1000;

Expression::Expression(Namespace_ptr ns)
:m_namespace(ns),m_object()
{
  defaultOperators();
}

/**
  * @param ns The namespace
  * @param p An initialised parser
  */
Expression::Expression(Namespace_ptr ns,const Parser& p)
:m_namespace(ns),m_object(),m_op(p.operator_name())
{
  fromParser(p);
  m_operators = p.operators();
}

/**
  * @param ns The namespace
  * @param p An initialised parser
  */
Expression::Expression(Namespace_ptr ns,const std::string& str)
:m_namespace(ns),m_object(),m_handler()
{
  defaultOperators();
  Parser p(m_operators);
  p.parse(str);
  fromParser(p);
}

/// Copy constructor
Expression::Expression(const Expression& expr)
:m_namespace(expr.m_namespace),
m_name(expr.m_name),
m_object(expr.m_object),
m_op(expr.m_op),
m_operators(expr.m_operators),
m_handler()
{
  for(const_iterator it = expr.begin();it!=expr.end();it++)
  {
    m_terms.push_back(boost::shared_ptr<Expression>(new Expression(**it)));
  }
}

Expression::~Expression()
{
}

void Expression::defaultOperators()
{
  m_operators.reset(new Operators());
  // Define binary operators. Put them in the reverse precedence order (from lower to higher prec.)
  std::vector<std::string> ops;
  ops.push_back(";");
  ops.push_back(",");
  //ops.push_back("=");
  ops.push_back("= != > >= < <=");
  ops.push_back("& | ^");
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

void Expression::fromParser(const Parser& p)
{
  m_op = p.operator_name();
  bool special_case = p.name() == "+";
  if (p.isFunct())
  {
    setFunction(p.name());
    for(Parser::iterator it=p.begin();it!= p.end();it++)
    {
      if (special_case &&(it->name() == "-" && it->size() == 1))
      {
        special_case = false;
        Expression* arg = new Expression(m_namespace,*(it->begin()));
        arg->m_op = "-";
        m_terms.push_back(boost::shared_ptr<Expression>(arg));
      }
      else
      {
        Expression* arg = new Expression(m_namespace,*it);
        m_terms.push_back(boost::shared_ptr<Expression>(arg));
      }
    }
  }
  else
  {
    setVariable(p.name());
  }
}

void Expression::setVariable(const std::string& aName)
{
  m_object = m_namespace->getVariable(aName);
  if (!m_object)
  {
    if (!m_namespace->addVariable("Scalar",aName))
    {
      std::cerr<<"Cannot create variable\n";
    }
    m_object = m_namespace->getVariable(aName);
    if (!m_object)
    {
      std::cerr<<"Empty object\n";
    }
  }
  m_name = aName;
}

void Expression::setFunction(const std::string& aName)
{
  m_object = m_namespace->getFunction(aName);
  m_name = aName;
}

/**
  * Set the expression object to be a text. The text will be owsned by
  * the expression
  * @param text The text
  * @return false if text is a coltrol word
  */
void Expression::setText(const std::string& text)
{
  m_object.reset(new TextObject(m_namespace.get(),text));
}

const std::string Expression::name()const
{
  if (!m_name.empty())
  {
    return m_name;
  }
  else if (m_object)
  {
    return m_object->name();
  }
  return "";
}

bool Expression::isBinary()const
{
  return m_operators->op_prec(name()) > 0 && size() > 1;
}

size_t Expression::precedence()const
{
  size_t prec = m_operators->op_prec(name());
  if (prec > 0) return prec;
  return s_max_precedence;
}

IVariable_ptr Expression::eval()const
{
  IObject_ptr retVal;
  Function_ptr fun = boost::dynamic_pointer_cast<Function>(m_object);
  if (fun)
  {
    std::vector<FunctionArgument> args;
    for(const_iterator it=begin();it!=end();it++)
    {
      args.push_back(FunctionArgument((**it).eval(),(**it).op()));
    }
    return fun->eval(args);
  }
  IVariable_ptr v = boost::dynamic_pointer_cast<IVariable>(m_object);
  if (!v)
  {
    throw std::runtime_error("Unexpected non-IVariable found");
  }
  Expression_ptr e = v->getExpression();
  return e ? e->eval() : v;
}

/**
  * Set a handler for the expression
  * @param eh The new handler
  */
boost::shared_ptr<ExpressionHandler> Expression::setHandler(ExpressionHandler* eh)
{
  if (&eh->expr() != this)
  {
    throw std::runtime_error("Setting handler to a wrong expression");
  }
  if (eh != m_handler.get())
  {
    m_handler.reset(eh);
    eh->init();
  }
  return getHandler();
}

void Expression::deleteTerm(int i)
{
  if (isVariable()) return;
  bool is_binary = isBinary();
  if (i < size())
  {
    m_terms.erase(m_terms.begin()+i);
  }
  if (is_binary && size() == 1)
  {
    //Expression tmp(*m_terms[0]);
    copyFrom(m_terms[0]);
  }
  if (!is_binary && size() == 0)
  {
    Expression* e = new Expression(getNamespace(),"_");
    m_terms.push_back(Expression_ptr(e));
  }

}

/// Make this a copy of expr
void Expression::copyFrom(const Expression& expr)
{
  m_namespace = expr.m_namespace;
  m_object = expr.m_object;
  m_name = expr.m_name;
  //m_op = expr.m_op;//?should operator be copied?
  m_operators = expr.m_operators;
  m_terms.clear();
  for(const_iterator it = expr.begin();it!=expr.end();it++)
  {
    m_terms.push_back(boost::shared_ptr<Expression>(new Expression(**it)));
  }
}

void Expression::rename(const std::string& aName)
{
    if (!m_object) return;
    if (size() == 0)
    {
      setVariable(aName);
    }
    else
    {
      setFunction(aName);
    }
    m_name = aName;
}

void Expression::clear()
{
  m_terms.clear();
  m_object.reset();
  m_op = "";
}

Expression_ptr Expression::addTerm(const std::string& op, Expression* expr)
{
  size_t prec = m_operators->op_prec(name());
  if (prec < 0)
  {
    return Expression_ptr();
  }
  else if (prec == 0)
  {
    if (op != ",") return Expression_ptr();
  }
  else if (m_operators->what_is_binary_name(op) != name())
  {
    return Expression_ptr();
  }

  Expression_ptr e(expr);
  m_terms.push_back(e);
  expr->m_op = op;
  return e;
}

boost::shared_ptr<Expression>
Expression::insertTerm(const Expression& afterMe,
                       const std::string& op,
                       Expression* expr)
{
  size_t prec = m_operators->op_prec(name());
  if (prec < 0)
  {
    return Expression_ptr();
  }
  else if (prec == 0)
  {
    if (op != ",") return Expression_ptr();
  }
  else if (m_operators->what_is_binary_name(op) != name())
  {
    return Expression_ptr();
  }

  for(iterator it = begin(); it != end(); ++it)
  {
    if (it->get() == &afterMe)
    {
      Expression_ptr e(expr);
      m_terms.insert(++it,e);
      expr->m_op = op;
      return e;
    }
  }
  return Expression_ptr();
}

bool isSpace(int c)
{
  return isspace(c) || (!isprint(c));
}

bool isSymbol(int c)
{
  return (!isspace(c)) && isprint(c);
}

void Expression::fromText(const std::string& text)
{
  clear();
  setFunction("Text");
  if (text.empty())
  {
    Expression* arg = new Expression(m_namespace);
    arg->setText(" ");
    m_terms.push_back(boost::shared_ptr<Expression>(arg));
  }
  std::string::const_iterator it = std::find_if(text.begin(),text.end(),isSymbol);
  
  while(it != text.end())
  {
    std::string::const_iterator it1 = std::find_if(it,text.end(),isSpace);
    if (*it == '\\')
    {// control word
      it1 = std::find(it,text.end(),'{');
      if (it1 == text.end())
      {
        throw std::invalid_argument("Expression::fromText: Syntax error");
      }
      std::string cmd = text.substr(it-text.begin(),it1-it);
      Str::trim(cmd);
      it = it1;
      it1 = std::find(it,text.end(),'}');
      if (it1 == text.end())
      {
        throw std::invalid_argument("Expression::fromText: Syntax error");
      }
      if (it1 - it > 1) 
      {
        // arg is the text inside curly brackets
        std::string arg = text.substr(it-text.begin()+1,it1-it-1);
        Str::trim(arg);
        if (cmd == "\\math")
        {
          Expression* e = new Expression(m_namespace,arg);
          m_terms.push_back(boost::shared_ptr<Expression>(e));
        }
        else if (cmd == "\\def")
        {
          std::vector<std::string> var = Str::split(arg,'=');
          if (var.size() > 0)
          {
            bool ok = m_namespace->defVariable("Scalar",var[0]);
            if (ok && var.size() > 1)
            {
              m_namespace->setVariable(var[0],var[1]);
            }
          }
        }
      }
      ++it1;
    }
    else
    {
      Expression* arg = new Expression(m_namespace);
      arg->setText(text.substr(it-text.begin(),it1-it));
      m_terms.push_back(boost::shared_ptr<Expression>(arg));
    }
    if (it1 == text.end()) break;
    it = std::find_if(it1,text.end(),isSymbol);
  }
}

void Expression::fromMath(const std::string& str)
{
  clear();
  Parser p(m_operators);
  p.parse(str);
  fromParser(p);
}

/** Make this expression a binary operation with operator op (function name) and
  * operands e1 and e2. If this expression must be a member of another
  * expression op1 is its operator (m_op)
  */
void Expression::makeBinary(Expression* e1,const std::string& op,Expression* e2,
                            const std::string& op1)
{
  clear();
  std::string fnName = m_operators->what_is_binary_name(op);
  if (fnName.empty()) fnName = "+";//?
  Expression* e11 = e1;
  if (fnName == "+" && e1->name() == "-")
  {
    e11 = new Expression((*e1)[0]);
    e11->m_op = "-";
    delete e1;
  }
  else
  {
    e1->m_op = "";
  }
  e2->m_op = op;
  m_op = op1;
  m_terms.push_back(Expression_ptr(e11));
  m_terms.push_back(Expression_ptr(e2));
  setFunction(fnName);
}

void Expression::makeFunction(const std::string& fnName,Expression* e,
                const std::string& op1)
{
  e->m_op = "";
  m_op = op1;
  m_terms.clear();
  m_terms.push_back(Expression_ptr(e));
  setFunction(fnName);
}

void Expression::makeVariable(const std::string& varName,
                const std::string& op1)
{
  m_op = op1;
  m_terms.clear();
  setVariable(varName);
}

void Expression::logPrint(const std::string& pads,bool logHandler)const
{
  std::string myPads = pads + "   ";
  if (m_terms.size())
  {
    std::cerr<<myPads<<op()<<'['<<name()<<']'<<"("<<'\n';
    for(size_t i=0;i<m_terms.size();i++)
      m_terms[i]->logPrint(myPads,logHandler);
    std::cerr<<myPads<<")";
  }
  else
    std::cerr<<myPads<<op()<<name();
  if (logHandler && m_handler)
  {
      std::cerr<<"  handler:"<<m_handler<<' ';
      m_handler->log();
  }
  std::cerr<<std::endl;
}

std::set<std::string> Expression::getVarNames()const
{
  std::set<std::string> res;
  if (isFunction())
  {
    for(const_iterator it=begin();it!= end(); ++it)
    {
      std::set<std::string> tmp = (**it).getVarNames();
      res.insert(tmp.begin(),tmp.end());
    }
  }
  else
  {
    if (!m_namespace->isNumber(name()))
    {
      res.insert(name());
    }
  }
  return res;
}

std::set<std::string> Expression::getFreeVarNames()const
{
  std::set<std::string> res;
  std::set<std::string> vars = getVarNames();
  std::set<std::string>::iterator it = vars.begin();
  for(; it != vars.end(); ++it)
  {
    if ( !m_namespace->hasValue(*it) )
    {
      res.insert(*it);
    }
  }
  return res;
}

/**
  * 
  */
bool Expression::isEquivalent(const Expression& expr)const
{
  return orderedString() == expr.orderedString();
}

void Expression::removeExtraBrackets()
{
  for(int i = 0; i < size(); ++i)
  {
    m_terms[i]->removeExtraBrackets();
  }

  if ((name().empty() || name() == "_") && size() == 1 )
  {
    copyFrom(m_terms[0]);
  }
}

std::string Expression::orderedString()const
{
  if (isVariable()) return name();
  std::multiset<std::string> srt;
  std::stringstream res;
  bool is_binary = isBinary();
  if (!is_binary)
  {
    res << name() << "(";
  }

  for(const_iterator it = begin(); it != end(); ++it)
  {
    std::string tmp_op = (**it).op();
    if (tmp_op.empty())
    {
      if (is_binary)
      {
        tmp_op = name();
      }
      else
      {
        tmp_op = ",";
      }
    }
    std::string tmp = tmp_op + "(" + (**it).orderedString() + ")";
    srt.insert(tmp);
  }
  std::multiset<std::string>::const_iterator s = srt.begin();
  for(;s!=srt.end();++s)
  {
    res << *s;
  }
  if (!is_binary)
  {
    res << ")";
  }
  return res.str();
}

/*--------------------------------------------------------*/


ExpressionHandler_ptr ExpressionHandler::deleteTerm(int i)
{
  Expression* par = parent();
  expr().deleteTerm(i);
  return expr().setHandler(create(expr(),par));
}

void ExpressionHandler::rename(const std::string& aname)
{
  expr().rename(aname);
}

void ExpressionHandler::clear()
{
  expr().clear();
}

Expression_ptr ExpressionHandler::addTerm(const std::string& op, Expression* ex)
{
  Expression_ptr e = expr().addTerm(op,ex);
  ex->setHandler(create(*ex,&expr()));
  return e;
}

Expression_ptr ExpressionHandler::insertTerm(const Expression& afterMe,
                                         const std::string& op, 
                                         Expression* ex)
{
  Expression_ptr e = expr().insertTerm(afterMe,op,ex);
  ex->setHandler(create(*ex,&expr()));
  return e;
}

ExpressionHandler_ptr ExpressionHandler::makeBinary(Expression* e1,const std::string& op,Expression* e2,
                const std::string& op1)
{
  expr().makeBinary(e1,op,e2,op1);
  return expr().setHandler(create(expr(),parent()));
}

void ExpressionHandler::makeFunction(const std::string& fnName,Expression* e,const std::string& op1)
{
  expr().makeFunction(fnName,e,op1);
  expr().setHandler(create(expr(),parent()));
}

void ExpressionHandler::makeVariable(const std::string& varName,const std::string& op1)
{
  expr().makeVariable(varName,op1);
  expr().setHandler(create(expr(),parent()));
}

void ExpressionHandler::copyFrom(boost::shared_ptr<Expression> ex)
{
  expr().copyFrom(*ex);
  expr().setHandler(create(expr(),parent()));
}

void ExpressionHandler::fromText(const std::string& text)
{
  expr().fromText(text);
  expr().setHandler(create(expr(),parent()));
}

void ExpressionHandler::fromMath(const std::string& str)
{
  expr().fromMath(str);
  expr().setHandler(create(expr(),parent()));
}

ExpressionHandler_ptr ExpressionHandler::removeExtraBrackets()
{
  expr().removeExtraBrackets();
  return expr().setHandler(create(expr(),parent()));
}

std::string Expression::str()const
{
  bool brackets = false;
  std::ostringstream res;
  std::string fnName = name();
  size_t prec = m_operators->op_prec(fnName);
  if (size() == 1 && m_operators->is_unary(fnName))
  {// unary operator
    res << fnName;
    if (m_operators->op_prec(m_terms[0]->name()) > 0)
    {
      brackets = true;
    }
  }
  else if (!prec)// !isBinary ?
  {// function with a name
    res << fnName;
    brackets = true;
  }

  if (m_terms.size())
  {
    if (brackets) res << '(' ;
    for(size_t i=0;i<m_terms.size();i++)
    {
      res << m_terms[i]->op();
      int prec1 = m_operators->op_prec(m_terms[i]->name());
      bool isItUnary = false;
      if (m_terms[i]->size() == 1 && m_operators->is_unary(m_terms[i]->name()))
      {
        prec1 = 0; // unary operator
        isItUnary = true;
      }
      bool bk = prec > 0 && prec1 > 0 && prec > prec1;
      if (bk) res << '(' ;
      if (isItUnary) res << ' ';
      res << m_terms[i]->str();
      if (bk) res <<')';
    }
    if (brackets) res << ')' ;
  }
  return res.str();
}

bool Expression::areAllVarsDefined()const
{
  std::set<std::string> vars = getVarNames();
  std::vector<IVariable_ptr> visObjects = m_namespace->getVars();
  std::vector<IVariable_ptr>::const_iterator it = visObjects.begin();
  for(;it != visObjects.end(); ++it)
  {
    std::string nam = (**it).name();
    std::set<std::string>::iterator v = vars.find(nam);
    if (v != vars.end())
    {
      vars.erase(v);
    }
  }

  return vars.empty();
}
