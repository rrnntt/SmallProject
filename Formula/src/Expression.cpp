#include "Formula/Expression.h"
#include "Formula/Function.h"
#include "Formula/FunctionFactory.h"
#include "Formula/Reference.h"

#include <sstream>

namespace Formula
{

  size_t Expression::max_precedence()
  {
    static const size_t s_max_precedence = 1000;
    return s_max_precedence;
  }

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
  :m_namespace(ns),m_object()
  {
    defaultOperators();
    Parser p(m_operators);
    p.parse(str);
    fromParser(p);
  }

  /// Copy constructor
  Expression::Expression(const Expression& expr)
  :m_namespace(expr.m_namespace),
  m_object(expr.m_object),
  m_name(expr.m_name),
  m_op(expr.m_op),
  m_operators(expr.m_operators)
  {
    for(const_iterator it = expr.begin();it!=expr.end();it++)
    {
      m_terms.push_back(boost::shared_ptr<Expression>(new Expression(**it)));
    }
  }

  Expression::Expression(const std::string& str)
  :m_namespace(new Namespace()),m_object()
  {
    defaultOperators();
    Parser p(m_operators);
    p.parse(str);
    fromParser(p);
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
    //ops.push_back(":");
    ops.push_back("= != > >= < <=");
    ops.push_back("&");
    ops.push_back("|");
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
      // create the arguments first
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
      // set the function object
      setFunction(p.name());
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
      if (m_namespace->isNumber(aName))
      {
        m_namespace->addVariable("Scalar",aName);
        m_namespace->setVariable(aName,aName);
        m_object = m_namespace->getVariable(aName);
      }

      if (!m_object)
      {
        throw std::invalid_argument("Variable "+aName+" is not defined.");
      }
    }
    m_name = aName;
  }

  void Expression::setFunction(const std::string& aName)
  {
    std::vector<Argument> args;
    for(iterator t=begin();t!=end();++t)
    {
      Object_ptr o = (**t).object();
      const Variable& v = (*o)();
      args.push_back(v);
    }

    // search local functions first
    Function_ptr fun_p = m_namespace->getFunction(aName);
    if (!fun_p)
    {
      // if not local create a global one
      Function* fun = FunctionFactory::instance().create(aName,args);
      if (fun)
      {
        fun_p.reset(fun);
      }
    }
    if (!fun_p)
    {
      throw std::runtime_error("Function "+aName+" does not exist");
    }
    // set function object
    m_object = fun_p;
    for(size_t i=0;i<args.size();++i)
    {
      args[i].inv = fun_p->get_inv(m_terms[i]->op());
    }
    fun_p->setArguments(args);
    m_name = aName;
  }

  size_t Expression::precedence()const
  {
    size_t prec = m_operators->op_prec(name());
    if (prec > 0) return prec;
    return max_precedence();
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
    std::cerr<<std::endl;
  }

  const Variable& Expression::eval()const
  {
    if (!m_object)
    {
      throw std::runtime_error("Function "+name()+" is undefined in expression "+str());
    }
    for(const_iterator t=begin();t!=end();++t)
    {
      (**t).eval();
    }
    return m_object->eval();
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
    std::vector<Variable_ptr> visObjects = m_namespace->getVars();
    std::vector<Variable_ptr>::const_iterator it = visObjects.begin();
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

} // Formula
