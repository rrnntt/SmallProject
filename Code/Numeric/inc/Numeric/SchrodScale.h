#ifndef NUMERIC_SCHRODSCALE_H
#define NUMERIC_SCHRODSCALE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT SchrodScale: public API::Algorithm
{
public:
  SchrodScale();
  virtual std::string name() const {return "SchrodScale";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_SCHRODSCALE_H
