#ifndef DATAOBJECTS_EVALCHEBFUN_H
#define DATAOBJECTS_EVALCHEBFUN_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT EvalChebfun: public API::Algorithm
{
public:
  EvalChebfun();
  virtual std::string name() const {return "EvalChebfun";}
protected:
  virtual void exec();
};

} // DATAOBJECTS

#endif // DATAOBJECTS_EVALCHEBFUN_H
