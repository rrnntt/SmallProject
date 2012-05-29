#ifndef NUMERIC_EVALCHEBFUN_H
#define NUMERIC_EVALCHEBFUN_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT EvalChebfun: public API::Algorithm
{
public:
  EvalChebfun();
  virtual std::string name() const {return "EvalChebfun";}
protected:
  virtual void exec();
};

} // Numeric

#endif // NUMERIC_EVALCHEBFUN_H
