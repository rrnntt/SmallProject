#ifndef DATAOBJECTS_ADDCHEBFUN_H
#define DATAOBJECTS_ADDCHEBFUN_H

#include "Numeric/DllExport.h"

#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT AddChebfun: public API::Algorithm
{
public:
  AddChebfun();
  virtual std::string name() const {return "AddChebfun";}

protected:
  void exec();
};

} // NUMERIC

#endif // DATAOBJECTS_ADDCHEBFUN_H
