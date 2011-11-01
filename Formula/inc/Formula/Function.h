#ifndef FUNCTION_H
#define FUNCTION_H

#include "Formula/Object.h"
#include "Formula/Variable.h"

#include <vector>

namespace Formula
{

  struct Argument
  {
    Argument(const Variable* new_var,int op = 0):inv(op),var(new_var){}
    Argument(const Variable& new_var,int op = 0):inv(op),var(&new_var){}
    int inv; // 1 for inverse operation
    const Variable* var;
    const Variable& operator ()() const{return *var;}
  };

  class FORMULA_EXPORT Function : public Object
  {
  public:
    Function():Object(){}
    Function(const std::string& oName):Object(oName){}
    Function(const Object& o):Object(o){}
    std::string type()const{return "Function";}
    virtual void setArguments(std::vector<Argument>& args);

    /* virtual public functions */

    /** Test argument types for compatibility with this function.
      * @param args A list of arguments in correct order.
      * @return 0 if test passed, otherwise index of first wrong argument + 1
      */
    virtual int testArguments(std::vector<Argument>& args) = 0;
    /** Defines a number for functions with the same precedence, e.g.
      * functions "= != > >= < <=" it should return "="->0, "!="->1, ">"->2, etc
      */
    virtual int get_inv(const std::string& op)const{return 0;}

  protected:
    // arguments are writable pointers for functions such as set
    std::vector<Argument> m_argv;
  };

  template<class ReturnType>
  class ReturnTypeFunction: public Function
  {
  public:
    virtual const Variable& operator()()const{return m_return;}
  protected:
    mutable ReturnType m_return;
  };

  typedef boost::shared_ptr<Function> Function_ptr;

} // Formula

#endif // FUNCTION_H
