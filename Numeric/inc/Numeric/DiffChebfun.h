#ifndef NUMERIC_DIFFCHEBFUN_H
#define NUMERIC_DIFFCHEBFUN_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT DiffChebfun: public API::Algorithm
{
public:
  DiffChebfun();
  virtual std::string name() const {return "DiffChebfun";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_DIFFCHEBFUN_H
