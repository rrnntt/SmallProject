#ifndef DATAOBJECTS_UNIFORMCHEBFUN_H
#define DATAOBJECTS_UNIFORMCHEBFUN_H

#include "DataObjects/DllExport.h"
#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT UniformChebfun: public API::Algorithm
{
public:
  UniformChebfun();
  virtual std::string name() const {return "UniformChebfun";}
protected:
  virtual void exec();
};

} // DATAOBJECTS

#endif // DATAOBJECTS_UNIFORMCHEBFUN_H
