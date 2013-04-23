#ifndef NUMERIC_COPYCHEBFUN_H
#define NUMERIC_COPYCHEBFUN_H

#include "Numeric/DllExport.h"

#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT CopyChebfun: public API::Algorithm
{
public:
  CopyChebfun();
  virtual std::string name() const {return "CopyChebfun";}

protected:
  void exec();
};

} // Numeric

#endif // NUMERIC_COPYCHEBFUN_H
