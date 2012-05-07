#ifndef NUMERIC_CREATECHEBFUN_H
#define NUMERIC_CREATECHEBFUN_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT CreateChebfun: public API::Algorithm
{
public:
  CreateChebfun();
  ~CreateChebfun(){}
  virtual std::string name() const {return "CreateChebfun";}
protected:
  virtual void exec();
};

} // namespace Numeric

#endif // NUMERIC_CREATECHEBFUN_H
