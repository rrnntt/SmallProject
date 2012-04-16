#ifndef DATAOBJECTS_ADDCHEBFUN_H
#define DATAOBJECTS_ADDCHEBFUN_H

#include "DataObjects/DllExport.h"

#include "API/Algorithm.h"

namespace DataObjects
{

class DATAOBJECTS_EXPORT AddChebfun: public API::Algorithm
{
public:
  AddChebfun();
  virtual std::string name() const {return "AddChebfun";}

protected:
  void exec();
};

} // DATAOBJECTS

#endif // DATAOBJECTS_ADDCHEBFUN_H
