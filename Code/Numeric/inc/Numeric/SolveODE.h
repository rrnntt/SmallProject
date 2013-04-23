#ifndef NUMERIC_SOLVEODE_H
#define NUMERIC_SOLVEODE_H

#include "Numeric/DllExport.h"
#include "API/Algorithm.h"

namespace Numeric
{

class NUMERIC_EXPORT SolveODE: public API::Algorithm
{
public:
  SolveODE();
  virtual std::string name() const {return "SolveODE";}
protected:
  virtual void exec();
};

} // NUMERIC

#endif // NUMERIC_SOLVEODE_H
