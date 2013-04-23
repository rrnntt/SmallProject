#ifndef FORMULA_REFERENCE_H
#define FORMULA_REFERENCE_H

#include "Formula/DllExport.h"
#include "Formula/Variable.h"

namespace Formula
{

class Namespace;

class FORMULA_EXPORT Reference: public Variable
{
public:
    Reference();
//    Reference(const std::string& oName);
//    Reference(const Variable& v);
    Reference(Variable_ptr var);
    Reference(const Reference& ref);
    void setName(const std::string& nm);
    std::string type() const;
    const Variable& operator()()const;
    const Variable& eval()const;

    std::string toString()const;
    bool setValue(const std::string&);
    bool setValue(const Variable&);
    bool setValue(const double*);
    void setRandomValue();
    Variable* clone()const;
    bool operator==(const Variable&);
    bool operator!=(const Variable&);
    bool compare(const Variable&,double absTol = -1., double relTol = -1.);
    bool isLiteral(const std::string&)const;

    void reset(Variable_ptr var);
  private:
    Variable_ptr m_var;
};

typedef boost::shared_ptr<Reference> Ref_ptr;

class FORMULA_EXPORT Address: public Reference
{
public:
  Address():Reference(){}
  Address(Variable_ptr var):Reference(var){}
  Address(const Address& ref):Reference(ref){}
  std::string type() const{return "Address";}
  Variable* clone()const{return new Address(*this);}
};

} // Formula

#endif // FORMULA_REFERENCE_H
