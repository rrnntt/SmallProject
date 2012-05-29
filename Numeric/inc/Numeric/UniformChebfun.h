#ifndef NUMERIC_UNIFORMCHEBFUN_H
#define NUMERIC_UNIFORMCHEBFUN_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT UniformChebfun: public API::Algorithm
{
public:
  UniformChebfun();
  virtual std::string name() const {return "UniformChebfun";}
protected:
  virtual void exec();
};

} // Numeric

#endif // NUMERIC_UNIFORMCHEBFUN_H
