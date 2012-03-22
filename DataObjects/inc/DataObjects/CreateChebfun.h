#ifndef DATAOBJECTS_CREATECHEBFUN_H
#define DATAOBJECTS_CREATECHEBFUN_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT CreateChebfun: public API::Algorithm
{
public:
  CreateChebfun();
  ~CreateChebfun(){}
  virtual std::string name() const {return "CreateChebfun";}
protected:
  virtual void exec();
};

} // namespace DataObjects
#endif // DATAOBJECTS_CREATECHEBFUN_H
