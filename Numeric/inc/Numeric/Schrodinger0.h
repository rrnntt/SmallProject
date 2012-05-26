#ifndef NUMERIC_SCHRODINGER0_H
#define NUMERIC_SCHRODINGER0_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT Schrodinger0: public API::Algorithm
{
public:
  Schrodinger0();
  virtual std::string name() const {return "Schrodinger0";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_SCHRODINGER0_H
