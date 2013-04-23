#ifndef FORMULA_BOOL_H_
#define FORMULA_BOOL_H_

#include "Formula/DllExport.h"
#include "Formula/Variable.h"

namespace Formula
{

  class FORMULA_EXPORT Bool: public Variable
  {
  public:
    Bool();
    Bool(const Bool&);
    Bool(bool b):m_value(b){}
    std::string toString()const;
    std::string type() const{return "Bool";}
    bool setValue(const std::string&);
    bool setValue(const Variable&);
    bool setValue(const double*);
    void setRandomValue();
    Variable* clone()const ;
    bool operator==(const Variable&);
    bool operator!=(const Variable&);
    bool compare(const Variable&,double absTol = -1., double relTol = -1.);
    bool isLiteral(const std::string&)const;

    /* Bool methods */
    operator bool()const{return m_value;}
    void setValue(bool);
    void And(const Bool&);
    void Or(const Bool&);
  private:
    bool m_value;
  };

} // Formula

#endif // FORMULA_BOOL_H_
