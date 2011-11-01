#ifndef DUMMYVAR_H
#define DUMMYVAR_H

#include "Formula/Variable.h"
#include <sstream>

class DummyVar: public Formula::Variable
{
public:
  virtual std::string type()const{return "DummyVar";}
  virtual std::string toString()const
  {
    std::ostringstream ostr;
    ostr << type() << ' ' << stuff;
    return ostr.str();
  }
  virtual bool setValue(const std::string&){return false;}
  virtual bool setValue(const Variable&){return false;}
  virtual bool setValue(const double*){return false;}
  virtual Variable* clone()const{return NULL;}
  bool operator==(const Variable& v)
  {
    return stuff == v.as<DummyVar>().stuff;
  }

  int stuff;
};

#endif // DUMMYVAR_H
