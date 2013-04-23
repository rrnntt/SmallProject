#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "IObject.h"
#include "parser.h"
#include "Namespace.h"

#include <vector>
#include <string>
#include <set>

class Expression;
class Operators;

class ExpressionHandler
{
public:
    ExpressionHandler(Expression& expr,Expression* parent):m_expr(expr),m_parent(parent){}
    //ExpressionHandler(const ExpressionHandler& eh):m_expr(eh.m_expr){}
    virtual ~ExpressionHandler(){}
    virtual std::string name()const = 0;
    virtual void init() = 0;
    virtual ExpressionHandler* create(Expression& expr,Expression* parent)const = 0;
    virtual void log()const{std::cerr<<this->name();}
    Expression& expr(){return m_expr;}
    Expression* parent(){return m_parent;}
    const Expression& expr()const{return m_expr;}
    const Expression* parent()const{return m_parent;}
    /* Editing */
    boost::shared_ptr<ExpressionHandler> deleteTerm(int i);
    void rename(const std::string& aname);
    void clear();
    boost::shared_ptr<Expression> addTerm(const std::string& op, Expression* expr);
    boost::shared_ptr<Expression> insertTerm(const Expression& afterMe,
                                             const std::string& op, 
                                             Expression* expr);
    boost::shared_ptr<ExpressionHandler> makeBinary(Expression* e1,const std::string& op,Expression* e2,
                    const std::string& op1 = "");
    void makeFunction(const std::string& fnName,Expression* e,const std::string& op1 = "");
    void makeVariable(const std::string& varName,const std::string& op1 = "");
    void copyFrom(boost::shared_ptr<Expression> expr);
    boost::shared_ptr<ExpressionHandler> removeExtraBrackets();

    void fromText(const std::string& text);
    void fromMath(const std::string& str);
protected:
  void reparent(Expression* parent){m_parent=parent;}
    Expression& m_expr;  ///< points to the handled expression
    Expression* m_parent;
};

typedef boost::shared_ptr<ExpressionHandler> ExpressionHandler_ptr;

class Expression
{
public:
    /// Default constructor
    Expression(Namespace_ptr nc);
    /// Constructor
    Expression(Namespace_ptr nc,const Parser& p);
    /// Constructor
    Expression(Namespace_ptr nc,const std::string& str);
    /// Copy constructor
    Expression(const Expression& expr);
    ~Expression();
    /// Return the terms
    std::vector<boost::shared_ptr<Expression> >& terms(){return m_terms;}
    const std::vector<boost::shared_ptr<Expression> >& terms()const{return m_terms;}
    /// Number of arguments
    int size()const{return m_terms.size();}
    /// Name of the expression
    const std::string name()const;
    /// The object
    IObject_ptr object()const{return m_object;}
    IObject_cptr const_object()const{return m_object;}
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
    /// Return a list of all variable names in the expression
    std::set<std::string> getVarNames()const;
    std::set<std::string> getFreeVarNames()const;
    std::string str()const;
    bool areAllVarsDefined()const;

    /// Evaluate this expression
    IVariable_ptr eval()const;
    /// Is this expression equivalent to another
    bool isEquivalent(const Expression& expr)const;

    typedef std::vector<boost::shared_ptr<Expression> >::iterator iterator;
    iterator begin(){return m_terms.begin();}
    iterator end(){return m_terms.end();}
    Expression& operator[](int i){return *m_terms.at(i);}
    typedef std::vector<boost::shared_ptr<Expression> >::const_iterator const_iterator;
    const_iterator begin()const{return m_terms.begin();}
    const_iterator end()const{return m_terms.end();}
    const Expression& operator[](int i)const{return *m_terms.at(i);}
    boost::shared_ptr<Expression> getTerm(int i){return m_terms.at(i);}

    /* Editing */
    void deleteTerm(int i);
    void rename(const std::string& aname);
    void reOp(const std::string& nop){m_op = nop;}
    void clear();
    boost::shared_ptr<Expression> addTerm(const std::string& op, Expression* expr);
    /// Insert new term after expression afterMe. afterMe must be one of the terms of this expression
    boost::shared_ptr<Expression> insertTerm(const Expression& afterMe,
                                             const std::string& op, 
                                             Expression* expr);
    void makeBinary(Expression* e1,const std::string& op,Expression* e2,
                    const std::string& op1 = "");
    void makeFunction(const std::string& fnName,Expression* e,
                    const std::string& op1 = "");
    void makeVariable(const std::string& varName,
                    const std::string& op1 = "");
    /// Make this a copy of expr
    void copyFrom(const Expression& expr);
    void copyFrom(boost::shared_ptr<Expression> expr){copyFrom(*expr);}

    void removeExtraBrackets();
    /// Fill in the expression from a text. This m_object will be function "Text",
    /// its arguments will be of type TextObject. 
    /// Control sequences ...
    void fromText(const std::string& text);
    void fromMath(const std::string& str);

    /* Handler */
    /// Returns the actual handler
    boost::shared_ptr<ExpressionHandler> setHandler(ExpressionHandler* eh);
    boost::shared_ptr<ExpressionHandler> getHandler()const{return m_handler;}
    void removeHandler()const{m_handler.reset();}

    /**
    * Print the expression into std::cerr to show its structure
    * @param pads Padding to make indentation
    */
    void logPrint(const std::string& pads = "",bool logHandler = false)const;
    std::string orderedString()const;

protected:
    void defaultOperators();
    void fromParser(const Parser& p);
    void setVariable(const std::string& aName);
    void setFunction(const std::string& aName);
    void setText(const std::string& text);

    Namespace_ptr m_namespace;
    std::string m_name;
    IObject_ptr m_object;             ///< points to the associated object, not owned
    std::string m_op;                 ///< operator
    std::vector<boost::shared_ptr<Expression> > m_terms; ///< arguments (terms)
    mutable boost::shared_ptr<ExpressionHandler> m_handler; ///< external handler for the expression

    boost::shared_ptr<Operators> m_operators;
    static const size_t s_max_precedence;///< Maximum precedence to be assigned to functions with names
private:
};

typedef boost::shared_ptr<Expression> Expression_ptr;
typedef boost::shared_ptr<const Expression> Expression_cptr;

#endif // EXPRESSION_H
