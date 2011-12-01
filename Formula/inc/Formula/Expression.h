#ifndef Expression_H
#define Expression_H

#include "Formula/DllExport.h"
#include "Formula/Namespace.h"
#include "Kernel/EParser.h"

class Operators;

namespace Formula
{

class Expression;
typedef boost::shared_ptr<Expression> Expression_ptr;
typedef boost::shared_ptr<const Expression> Expression_cptr;

class FORMULA_EXPORT Expression
{
public:
  /// Default constructor
  Expression(Namespace_ptr nc);
  /// Constructor
  Expression(Namespace_ptr nc,const Kernel::EParser& p);
  /// Constructor
  Expression(Namespace_ptr nc,const std::string& str);
  /// Constructor. str must contain only constants
  Expression(const std::string& str);
  /// Copy constructor
  Expression(const Expression& expr);
  /// Destructor
  virtual ~Expression();

  /// Return the terms
  const std::vector<Expression_ptr>& terms()const{return m_terms;}
  /// Number of arguments
  size_t size()const{return m_terms.size();}
  /// Name of the expression
  const std::string name()const;
  /// The object
  Object_ptr object()const{return m_object;}
  Object_cptr const_object()const{return m_object;}
  /// The operator
  std::string op()const{return m_op;}
  /// Return binary operators
  boost::shared_ptr<const Operators> operators()const{return m_operators;}
  Namespace_ptr getNamespace()const{return m_namespace;}
  /// Is this a binary operator
  bool isBinary()const;
  bool isFunction()const{return size() > 0;}
  bool isVariable()const{return size() == 0;}
  bool isEquation()const{return name() == "=";}
  size_t precedence()const;

  typedef std::vector<Expression_ptr>::const_iterator const_iterator;
  const_iterator begin()const{return m_terms.begin();}
  const_iterator end()const{return m_terms.end();}
  const Expression& operator[](int i)const{return *m_terms.at(i);}
  Expression_cptr getTerm(int i){return m_terms.at(i);}

  const Variable& eval()const;
  /// Return a list of all variable names in the expression
  std::set<std::string> getVarNames()const;
  std::set<std::string> getFreeVarNames()const;
  std::string str()const;
  bool areAllVarsDefined()const;

  /**
  * Print the expression into std::cerr to show its structure
  * @param pads Padding to make indentation
  */
  void logPrint(const std::string& pads = "",bool logHandler = false)const;
  std::string orderedString()const;

protected:
  //  methods //
  /// Maximum precedence to be assigned to functions with names
  static size_t max_precedence();
  void defaultOperators();
  void fromParser(const Kernel::EParser& p);
  void setVariable(const std::string& aName);
  void setFunction(const std::string& aName);

  std::vector<Expression_ptr>& terms(){return m_terms;}
  typedef std::vector<Expression_ptr>::iterator iterator;
  iterator begin(){return m_terms.begin();}
  iterator end(){return m_terms.end();}
  Expression& operator[](int i){return *m_terms.at(i);}

  //  data //
  Namespace_ptr m_namespace;        ///< Namespace where the expression gets its variables and functions from
  Object_ptr m_object;             ///< points to the associated object, not owned
  std::string m_name;               ///< object's name (name of a variable or function)
  std::string m_op;                 ///< operator (+ - * , ...)
  std::vector<Expression_ptr> m_terms; ///< arguments (terms)

  Operators_ptr m_operators;           ///< Operators that can be used in this expression

};

} // Formula

#endif // Expression_H
