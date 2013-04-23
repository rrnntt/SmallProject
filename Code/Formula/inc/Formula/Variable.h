#ifndef IVARIABLE_H
#define IVARIABLE_H

#include "Formula/DllExport.h"
#include "Formula/Object.h"
#include <stdexcept>

namespace Formula
{

  class Expression;
  class Function;

class FORMULA_EXPORT Variable: public Object
{
public:
  Variable():Object(){}
  Variable(const std::string& oName):Object(oName){}
  Variable(const Variable& v):Object(v){}
  void setName(const std::string& nm);
  const Variable& operator()()const{return *this;}
  const Variable& eval()const{return *this;}

  virtual std::string toString()const = 0;
  virtual bool setValue(const std::string&) = 0;
  virtual bool setValue(const Variable&) = 0;
  virtual bool setValue(const double*) = 0;
  virtual void setRandomValue(){}
  virtual Variable* clone()const = 0;
  virtual bool operator==(const Variable&);
  virtual bool operator!=(const Variable&);
  virtual bool compare(const Variable&,double absTol = -1., double relTol = -1.);
  virtual bool isLiteral(const std::string&)const{return false;}
  template<class DerivedVar>
  const DerivedVar& as()const{return dynamic_cast<const DerivedVar&>(myReference());}
  template<class DerivedVar>
  DerivedVar& as(){return dynamic_cast<DerivedVar&>(myReference());}

  virtual void setExpression(boost::shared_ptr<Expression> expr);
  boost::shared_ptr<Expression> getExpression()const{return m_expr;}
  virtual void clear(){m_expr.reset();}

protected:
  virtual const Variable& myReference()const{return *this;}
  virtual Variable& myReference(){return *this;}
  boost::shared_ptr<Expression> m_expr;
};

typedef boost::shared_ptr<Variable> Variable_ptr;
typedef boost::shared_ptr<const Variable> Variable_cptr;

inline std::ostream& operator<<(std::ostream& ostr,const Variable& o)
{
  ostr << o.toString();
  return ostr;
}

inline void Variable::setName(const std::string& nm)
{
  Object::setName(nm);
  if (isLiteral(nm)) this->setValue(nm);
}

inline bool Variable::operator==(const Variable&)
{
  throw std::runtime_error("Operator == is not implemented for this object");
}

inline bool Variable::operator!=(const Variable& v)
{
  return !(*this == v);
}

inline bool Variable::compare(const Variable& obj,double, double )
{
  return this->operator ==(obj);
}

} // Formula

#endif // IVARIABLE_H
