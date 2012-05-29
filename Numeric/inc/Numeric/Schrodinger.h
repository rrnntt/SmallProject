#ifndef NUMERIC_SCHRODINGER_H
#define NUMERIC_SCHRODINGER_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT Schrodinger: public API::Algorithm
{
public:
  Schrodinger();
  virtual std::string name() const {return "Schrodinger";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_SCHRODINGER_H
