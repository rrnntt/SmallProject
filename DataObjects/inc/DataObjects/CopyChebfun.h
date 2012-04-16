#ifndef DATAOBJECTS_COPYCHEBFUN_H
#define DATAOBJECTS_COPYCHEBFUN_H

#include "DataObjects/DllExport.h"

#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT CopyChebfun: public API::Algorithm
{
public:
  CopyChebfun();
  virtual std::string name() const {return "CopyChebfun";}

protected:
  void exec();
};

} // DATAOBJECTS

#endif // DATAOBJECTS_COPYCHEBFUN_H
